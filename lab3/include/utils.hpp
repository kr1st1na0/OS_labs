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

const int MAP_SIZE = 1024;

constexpr const char *SEMAPHORE_NAME_1 = "/semaphore_1";
constexpr const char *SHARED_MEMORY_NAME_1 = "/shared_memory_1";

constexpr const char *SEMAPHORE_NAME_2 = "/semaphore_2";
constexpr const char *SHARED_MEMORY_NAME_2 = "/shared_memory_2";

sem_t* OpenSemaphore(const char *name, int value);
int OpenSharedMemory(const char *name, const int size);
char* MapSharedMemory(const int size, int fd);
pid_t CreateChildProcess();
bool CheckString(const std::string_view str);