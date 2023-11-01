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

    const char *SEMAPHORE_NAME = "semaphore";
    const int MAP_SIZE = 1024;
    const char *SHARED_MEMORY_NAME = "/shared_memory";

    sem_t* semptr = sem_open(SEMAPHORE_NAME, 0);
    int shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0);
    char* memptr = (char*)mmap(nullptr, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
    
    // sem_t* semptr2 = sem_open("semaphore2", 0);
    int shared_memory_fd2 = shm_open("/shared_memory2", O_RDWR, 0);
    char* memptr2 = (char*)mmap(nullptr, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd2, 0);

    sem_wait(semptr);

    std::string str(memptr);
    while (std::getline(std::cin, str)) {
        //! std::cout << str << std::endl;
        if (CheckString(str)) {
            fout << str << std::endl;
        } else {
            // sem_wait(semptr2);
            strcpy(memptr2, str.c_str());
            memptr2 += str.size() + 1;
            // sem_post(semptr2);
        }
    }

    sem_post(semptr);
    sem_close(semptr);
    exit(EXIT_SUCCESS);  
}
