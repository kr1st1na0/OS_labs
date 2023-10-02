#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ext/stdio_filebuf.h>

void createPipe(int fd[2]);
pid_t createChildProcess();
std::stringstream readFromPipe (int fd);
bool checkString(const std::string &str);