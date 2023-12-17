#include "nodeRoutine.hpp"

std::string Node::Ping(int _id) {
    std::string ans = "Ok: 0";
    if (_id == id) {
        ans = "Ok: 1";
        return ans;
    } else if (auto it = children.find(_id); it != children.end()) {
        std::string msg = "ping " + std::to_string(_id);
        SendMessage(it->second.get(), msg);          
        if (auto msg = ReceiveMessage(children[_id].get()); msg.has_value(), msg == "Ok: 1") {
            ans = *msg;
        }
        return ans;
    }
    return ans;
} 

std::string Node::Create(int idChild, const std::string& programPath) {
    std::string programName = programPath.substr(programPath.find_last_of("/") + 1);
    children[idChild] = std::make_unique<zmq::socket_t>(context, ZMQ_REQ);
    
    int newPort = Bind(children[idChild].get(), idChild);
    childrenPort[idChild] = newPort;
    int pid = fork();
    
    if (pid == 0) { // Child process
        execl(programPath.c_str(), programName.c_str(), std::to_string(idChild).c_str(), std::to_string(newPort).c_str(), nullptr);
    } else { // Parent process
        std::string pidChild = "Error: couldn't connect to child";
        children[idChild]->setsockopt(ZMQ_SNDTIMEO,  3000);
        SendMessage(children[idChild].get(), "pid");
        if (auto msg = ReceiveMessage(children[idChild].get()); msg.has_value()) {
            pidChild = *msg;
        }
        return "Ok: " + pidChild;
    }
    return 0;
}

std::string Node::Pid() {
    return std::to_string(getpid());
}

std::string Node::Send(const std::string& str, int id) {
    if (children.size() == 0) {
        return "Error: Not found";
    } else if (auto it = children.find(id); it != children.end()) {
        if (SendMessage(it->second.get(), str)) {
            std::string ans = "Error: Not found";
            if (auto msg = ReceiveMessage(children[id].get()); msg.has_value()) {
                ans = *msg;
            }
            return ans;
        }
    } else {
        std::string ans = "Error: Not found";
        for (auto& child : children) {
            std::string msg = "send " + std::to_string(id) + " " + str;
            if (SendMessage(child.second.get(), msg)) {
                if (auto msg = ReceiveMessage(child.second.get()); msg.has_value()) {
                    ans = *msg;
                }
            }
        }
        return ans;
    }
    return 0;
}

std::string Node::Kill() {
    std::string ans;
    for (auto& child : children) {
        std::string msg = "kill";
        if (SendMessage(child.second.get(), msg)) {
            if (auto tmp = ReceiveMessage(child.second.get()); tmp.has_value()) {
                msg = *tmp;
            }
            if (ans.size() > 0) {
                ans = ans + " " + msg;
            } else {
                ans =  msg;
            }
        }
        Unbind(child.second.get(), childrenPort[child.first]);
        child.second->close();
    }
    children.clear();
    childrenPort.clear();
    return ans;
}