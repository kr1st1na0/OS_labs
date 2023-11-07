#include "utils.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Not enough arguments");
        exit(EXIT_FAILURE);
    }

    const char *fileName = argv[1];
    std::ofstream fout(fileName, std::ios::app);
    if (!fout.is_open()) {
        perror("Couldn't open the file");
        exit(EXIT_FAILURE);
    }

    sem_t *semptr1 = OpenSemaphore(SEMAPHORE_NAME_1, 0);
    int shared_memory_fd1 = OpenSharedMemory(SHARED_MEMORY_NAME_1, MAP_SIZE);
    char *memptr1 = MapSharedMemory(MAP_SIZE, shared_memory_fd1);

    sem_t *semptr2 = OpenSemaphore(SEMAPHORE_NAME_2, 0);
    int shared_memory_fd2 = OpenSharedMemory(SHARED_MEMORY_NAME_2, MAP_SIZE);
    char* memptr2 = MapSharedMemory(MAP_SIZE, shared_memory_fd2);

    while (1) {
        sem_wait(semptr1);
        std::string_view str(memptr1);
        if (str.empty()) {
            sem_post(semptr2);
            break;
        }
        if (CheckString(str)) {
            fout << str << std::endl;
        } else {
            strcpy(memptr2, "ERROR_STRING");
        }
        sem_post(semptr2);
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

    exit(EXIT_SUCCESS);  
}
