#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <ext/stdio_filebuf.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

sem_t* OpenSemaphore(const char *SEMAPHORE_NAME);
int OpenSharedMemory(const char *SHARED_MEMORY_NAME, const int MAP_SIZE);
char* MapSharedMemory(const int MAP_SIZE, int sh_fd);
pid_t CreateChildProcess();
bool CheckString(const std::string &str);