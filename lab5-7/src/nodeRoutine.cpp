#include "nodeRoutine.hpp"

std::string Node::Ping(int _id) {
    std::string ans = "Ok: 0";
    if (_id == id) {
        ans = "Ok: 1";
        return ans;
    } else if (auto it = children.find(_id); it != children.end()) {
        std::string msg = "ping " + std::to_string(_id);
        SendMessage(it->second, msg);
        try{
            msg  = ReceiveMessage(children[_id]);
            if (msg == "Ok: 1")
                ans = msg;
        } catch(int) {}
        return ans;
    }
    return ans;
} 

std::string Node::Create(int idChild, const std::string& programPath) {
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

std::string Node::Pid() {
    return std::to_string(getpid());
}

std::string Node::Send(const std::string& str, int id) {
    if (children.size() == 0) {
        return "Error: Not found";
    } else if (auto it = children.find(id); it != children.end()) {
        if (SendMessage(it->second, str)) {
            std::string ans;
            try{
                ans = ReceiveMessage(children[id]);
            } catch(int) {
                ans = "Error: Not found";
            }
            return ans;
        }
    } else {
        std::string ans = "Error: Not found";
        for (auto& child : children) {
            std::string msg = "send " + std::to_string(id) + " " + str;
            if (SendMessage(child.second, msg)) {
                try {
                    msg = ReceiveMessage(child.second);
                } catch(int) {
                    msg = "Error: Not found";
                }
                ans = msg;
            }
        }
        return ans;
    }
    return 0;
}

std::string Node::Kill() {
    std::string ans;
    for (auto& child : children) {
        if (Ping(child.first) == "Ok: 1") {
            std::string msg = "kill";
            SendMessage(child.second, msg);
            try {
                msg = ReceiveMessage(child.second);
                if(ans.size() > 0)
                    ans = ans + " " + msg;
                else
                    ans =  msg;
            } catch(int) {}
        }
        Unbind(child.second, childrenPort[child.first]);
        child.second->close();
    }
    children.clear();
    childrenPort.clear();
    return ans;
}