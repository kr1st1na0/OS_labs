#include "parent.hpp"
#include "utils.hpp"

void ParentProcess(const char *pathToChild) {

    std::string fileName;
    getline(std::cin, fileName);

    sem_t *semptr1 = OpenSemaphore(SEMAPHORE_NAME_1, 0);
    int shared_memory_fd1 = OpenSharedMemory(SHARED_MEMORY_NAME_1, MAP_SIZE);
    char *memptr1 = MapSharedMemory(MAP_SIZE, shared_memory_fd1);

    sem_t *semptr2 = OpenSemaphore(SEMAPHORE_NAME_2, 0);
    int shared_memory_fd2 = OpenSharedMemory(SHARED_MEMORY_NAME_2, MAP_SIZE);
    char* memptr2 = MapSharedMemory(MAP_SIZE, shared_memory_fd2);

    std::string str;
    std::vector<std::string> errorStrings;

    int pid = CreateChildProcess();
    if (pid != 0) { // Parent process
        while(getline(std::cin, str)) {
            strcpy(memptr1, str.c_str());
            sem_post(semptr1); 
            
            sem_wait(semptr2);
            if (strcmp(memptr2, "ERROR_STRING") == 0) {
                errorStrings.push_back(str);
                strcpy(memptr2, "");
            }
        }
        strcpy(memptr1, "");
        sem_post(semptr1);
    } else { // Child process
        if (execlp(pathToChild, pathToChild, fileName.c_str(), nullptr) == -1) { // to child.cpp
            perror("Error with execlp");
            exit(EXIT_FAILURE);
        } 
    }

    for (const std::string &err : errorStrings) {
        std::cout << "ERROR with string: " << err << std::endl;
    }
        
    sem_close(semptr1);
    sem_unlink(SEMAPHORE_NAME_1);
    shm_unlink(SHARED_MEMORY_NAME_1);
    munmap(memptr1, MAP_SIZE);
    close(shared_memory_fd1);

    sem_close(semptr2);
    sem_unlink(SEMAPHORE_NAME_2);
    shm_unlink(SHARED_MEMORY_NAME_2);
    munmap(memptr2, MAP_SIZE);
    close(shared_memory_fd2);
}