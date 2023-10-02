#include "utils.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Not enough arguments");
        exit(EXIT_FAILURE);
    }

    const char *fileName = argv[1];
    std::ofstream fout(fileName);
    if (!fout.is_open()) {
        perror("Couldn't open the file");
        exit(EXIT_FAILURE);
    }
    
    std::string str;
    while (std::getline(std::cin, str)) {
        if (checkString(str)) {
            fout << str << '\n';
        } else {
            std::string error = "ERROR with string: " + str;
            std::cout << error << std::endl;
        }
    }
    
    fout.close();  
    exit(EXIT_SUCCESS);  
}