#pragma once

#include <iostream>
#include <sstream>
#include <unordered_map>
#include "unistd.h"

#include "socketRoutine.hpp"

class Node{
private:
    zmq::context_t context;
public:
    std::unordered_map<int, zmq::socket_t*> children;
    std::unordered_map<int, int> childrenPort;
    int id;
    zmq::socket_t parent;
    int parentPort;

    Node(int _id, int _parentPort = -1): id(_id), parent(context, ZMQ_REP), parentPort(_parentPort) {
        if(_id != -1) {
            Connect(&parent, _parentPort);
        }
    }

    std::string Ping(int _id) {
        std::string ans = "Ok: 0";
        if (_id == id) {
            ans = "Ok: 1";
            return ans;
        } else if (children.find(_id) != children.end()) {
            std::string msg = "ping " + std::to_string(_id);
            SendMessage(children[_id], msg);
            try{
                msg  = ReceiveMessage(children[_id]);
                if (msg == "Ok: 1")
                    ans = msg;
            } catch(int) {}
            return ans;
        }
        return ans;
    } 

    std::string Create(int idChild, std::string programPath) {
        std::string programName = programPath.substr(programPath.find_last_of("/") + 1);
        children[idChild] = new zmq::socket_t(context, ZMQ_REQ);
        
        int newPort = Bind(children[idChild], idChild);
        childrenPort[idChild] = newPort;
        int pid = fork();
        
        if (pid == 0) { // Child process
            execl(programPath.c_str(), programName.c_str(), std::to_string(idChild).c_str(), std::to_string(newPort).c_str(), nullptr);
        } else { // Parent process
            std::string pidChild;
            try{
                children[idChild]->setsockopt(ZMQ_SNDTIMEO,  3000);
                SendMessage(children[idChild], "pid");
                pidChild = ReceiveMessage(children[idChild]);
            } catch(int) {
                pidChild = "Error: couldn't connect to child";
            }
            return "Ok: " + pidChild;
        }
        return 0;
    }

    std::string Pid() {
        return std::to_string(getpid());
    }

    std::string Send(std::string str, int _id) {
        if (children.size() == 0) {
            return "Error: Not found";
        } else if (children.find(_id) != children.end()) {
            if(Ping(_id) == "Ok: 1") {
                SendMessage(children[_id], str);
                std::string ans;
                try{
                    ans = ReceiveMessage(children[_id]);
                } catch(int) {
                    ans = "Error: Not found";
                }
                return ans;
            }
        } else {
            std::string ans = "Error: Not found";
            for (auto& child : children) {
                if (Ping(child.first) == "Ok: 1") {
                    std::string msg = "send " + std::to_string(_id) + " " + str;
                    SendMessage(children[child.first], msg);
                    try {
                        msg = ReceiveMessage(children[child.first]);
                    } catch(int) {
                        msg = "Error: Not found";
                    }
                    if (msg != "Error: Not found") {
                        ans = msg;
                    }
                }
            }
            return ans;
        }
        return 0;
    }

    std::string Kill() {
        std::string ans;
        if (children.size() > 0) {
            for (auto& child: children ) {
                if (Ping(child.first) == "Ok: 1") {
                    std::string msg = "kill";
                    SendMessage(children[child.first], msg);
                    try {
                        msg = ReceiveMessage(children[child.first]);
                        if(ans.size() > 0)
                            ans = ans + " " + msg;
                        else
                            ans =  msg;
                    } catch(int) {}
                }
                Unbind(children[child.first], childrenPort[child.first]);
                children[child.first]->close();
            }
            children.clear();
            childrenPort.clear();
        }
        return ans;
    }
};