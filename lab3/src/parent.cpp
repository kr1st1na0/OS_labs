#include "parent.hpp"

void ParentProcess(const char *pathToChild) {

    std::string fileName;
    getline(std::cin, fileName);

    const char *SEMAPHORE_NAME = "semaphore";
    const int MAP_SIZE = 1024;
    const char *SHARED_MEMORY_NAME = "/shared_memory";

    sem_t *semptr = OpenSemaphore(SEMAPHORE_NAME);
    int shared_memory_fd = OpenSharedMemory(SHARED_MEMORY_NAME, MAP_SIZE);
    char *memptr = MapSharedMemory(MAP_SIZE, shared_memory_fd);

    // sem_t *semptr2 = OpenSemaphore("semaphore2");
    int shared_memory_fd2 = OpenSharedMemory("/shared_memory2", MAP_SIZE);
    char* memptr2 = MapSharedMemory(MAP_SIZE, shared_memory_fd2);

    int pid = CreateChildProcess();
    if (pid != 0) { // Parent process
        std::string str;
        while(getline(std::cin, str)) {
            str += "\n";
            sem_wait(semptr);
            //? write(shared_memory_fd, str.c_str(), str.length());
            strcpy(memptr, str.c_str());
            memptr += str.size() + 1;
            sem_post(semptr);
        }
        int status;
        wait(&status);
        // wait for child process
        sem_wait(semptr);
        // sem_wait(semptr2);
        
        std::string error(memptr2);
        while (std::getline(std::cin, error)) {
            //! std::cout << "parent.cpp" << std::endl; 
            std::cout << "Error: " << error << std::endl;
        }
        sem_post(semptr);
        //sem_post(semptr2);
        
    } else { // Child process
        if (execlp(pathToChild, pathToChild, fileName.c_str(), nullptr) == -1) { // to child.cpp
            perror("Error with execlp");
            exit(EXIT_FAILURE);
        } 
    }

    // sem_close(semptr2);
    shm_unlink("/shared_memory2");
    munmap(memptr2, MAP_SIZE);
    close(shared_memory_fd2);
        
    sem_close(semptr);
    sem_unlink(SEMAPHORE_NAME);
    shm_unlink(SHARED_MEMORY_NAME);
    munmap(memptr, MAP_SIZE);
    close(shared_memory_fd);
}
