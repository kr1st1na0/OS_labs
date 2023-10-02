#include "parent.hpp"

void parentProcess(const char *pathToChild) {
    std::string fileName;
    getline(std::cin, fileName);

    int fd1[2], fd2[2];
    createPipe(fd1);
    createPipe(fd2);
    
    int pid = createChildProcess();
    if (pid != 0) { // Parent process
        close(fd1[0]);
        close(fd2[1]);
        
        std::string str;
        while (getline(std::cin, str)) {
            str += "\n";
            write(fd1[1], str.c_str(), str.length()); // from str to fd1[1]
        }
        close(fd1[1]);

        std::stringstream output = readFromPipe(fd2[0]);
        while(std::getline(output, str)) {
            std::cout << str << std::endl;
        }
        close(fd2[0]);
    } else { // Child process
        close(fd1[1]);
        close(fd2[0]);

        if (dup2(fd1[0], STDIN_FILENO) == -1 || dup2(fd2[1], STDOUT_FILENO) == -1) {
            perror("Error with dup2");
            exit(EXIT_FAILURE);
        }
        
        if (execlp(pathToChild, pathToChild, fileName.c_str(), nullptr) == -1) { // to child.cpp
            perror("Error with execlp");
            exit(EXIT_FAILURE);
        } 
    }
}