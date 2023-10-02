#include "utils.hpp"

void createPipe(int fd[2]) {
    if (pipe(fd) == -1) {
        perror("Couldn't create pipe");
        exit(EXIT_FAILURE);
    }
}

pid_t createChildProcess() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Couldn't create child process");
        exit(EXIT_FAILURE);
    }
    return pid;
}

std::stringstream readFromPipe (int fd) {
    constexpr int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE] = "";
    // char c;
    std::stringstream stream;
    while(true) {       
        int t = read(fd, &buffer, BUFFER_SIZE);
        // int t = read(fd, &c, sizeof(char)); 
        if (t == -1) {
            perror("Couldn't read from pipe");
            exit(EXIT_FAILURE);
        } else if (t > 0) {
            stream << buffer;    
            // stream << c;
        } else {
            return stream;
        }
    }
}

bool checkString(const std::string &str) {
    if (str[str.size() - 1] == '.' || str[str.size() - 1] == ';') {
        return true;
    }
    return false;
}