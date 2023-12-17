#pragma once

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <optional>
#include <memory>
#include "unistd.h"

#include "socketRoutine.hpp"

class Node{
private:
    zmq::context_t context;
public:
    std::unordered_map<int, std::unique_ptr<zmq::socket_t> > children;
    std::unordered_map<int, int> childrenPort;
    int id;
    zmq::socket_t parent;
    int parentPort;

    Node(int _id, int _parentPort = -1): id(_id), parent(context, ZMQ_REP), parentPort(_parentPort) {
        if(_id != -1) {
            Connect(&parent, _parentPort);
        }
    }

    std::string Ping(int _id);
    std::string Create(int idChild, const std::string& programPath);
    std::string Pid();
    std::string Send(const std::string& str, int _id);
    std::string Kill();
};