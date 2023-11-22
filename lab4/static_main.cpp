#include "realizations.hpp"

#include <iostream>

void Task(const std::string& command) {
    if (command == "1") {
        float arg1, arg2, arg3;
        std::cin >> arg1 >> arg2 >> arg3;
        float result = SinIntegral(arg1, arg2, arg3);
        std::cout << "Result of integral = " << result << std::endl;
    } else if (command == "2") {
        int arg1, arg2;
        std::cin >> arg1 >> arg2;
        int result = PrimeCount(arg1, arg2);
        std::cout << "Count of prime numbers = " << result << std::endl;
    } else {
        std::cout << "Invalid command" << std::endl;
    }
}

int main() {
    std::string command;
    while(true) {
        std::cout << "Enter the command (0 - exit): ";
        std::cin >> command;
        if (command == "0") {
            break;
        }
        Task(command);
    }
    return 0;
}