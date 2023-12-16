#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <optional>

#include <zmq.hpp>

int Bind(zmq::socket_t *socket, int id);
void Unbind(zmq::socket_t *socket, int port);

void Connect(zmq::socket_t *socket, int port);
void Disconnect(zmq::socket_t *socket, int port);

bool SendMessage(zmq::socket_t *socket, const std::string& msg);
std::optional<std::string> ReceiveMessage(zmq::socket_t *socket);