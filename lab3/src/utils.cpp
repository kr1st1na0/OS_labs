#include "utils.hpp"

sem_t* OpenSemaphore(const char *name, int value) {
    sem_t *semptr = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, value);
    if (semptr == SEM_FAILED){
        perror("Couldn't open the semaphore");
        exit(EXIT_FAILURE);
    }
    return semptr;
}

int OpenSharedMemory(const char *name, const int size) {
    int sh_fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (sh_fd == -1) {
        perror("Couldn't create memory shared object");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(sh_fd, size) == -1) {
        perror("Couldn't truncate a file");
        exit(EXIT_FAILURE);
    }
    return sh_fd;
}

char* MapSharedMemory(const int size, int fd) {
    char *memptr = (char*)mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memptr == MAP_FAILED) {
        perror("Error with file mapping");
        exit(EXIT_FAILURE);
    }
    return memptr;
}

pid_t CreateChildProcess() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Couldn't create child process");
        exit(EXIT_FAILURE);
    }
    return pid;
}

bool CheckString(const std::string_view str) {
    if (str[str.size() - 1] == '.' || str[str.size() - 1] == ';') {
        return true;
    }
    return false;
}