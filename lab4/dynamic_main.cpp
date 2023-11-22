#include "utils.hpp"

int main() {
    const char *pathToLib1 = getenv("PATH_TO_LIB1");
    const char *pathToLib2 = getenv("PATH_TO_LIB2");
    // bash: export PATH_TO_LIB1="/home/kristinab/ubuntu_main/OS_labs/build/lab4/librealization1.so"
    // bash: export PATH_TO_LIB2="/home/kristinab/ubuntu_main/OS_labs/build/lab4/librealization2.so"

    void* libraryHandle = LoadLibrary(pathToLib1);
    SinIntegralFunc SinIntegral = (SinIntegralFunc)dlsym(libraryHandle, "SinIntegral");
    PrimeCountFunc PrimeCount = (PrimeCountFunc)dlsym(libraryHandle, "PrimeCount");

    std::string command;
    while(true) {
        std::cout << "Enter the command (0 - switch realization, e - exit): ";
        std::cin >> command;
        if (command == "e") {
            break;
        } else if (command == "0") {
            std::cout << "Enter the library (1 or 2): ";
            std::cin >> command;
            if (command == "1") {
                libraryHandle = LoadLibrary(pathToLib1);
            } else if (command == "2") {
                libraryHandle = LoadLibrary(pathToLib2);
            } else {
                std::cout << "Invalid library" << std::endl;
            }
            SinIntegral = (SinIntegralFunc)dlsym(libraryHandle, "SinIntegral");
            PrimeCount = (PrimeCountFunc)dlsym(libraryHandle, "PrimeCount");
        } else {
            if (command == "1") {
                std::cout << "SinIntegral function:" << std::endl;
                float arg1, arg2, arg3;
                std::cin >> arg1 >> arg2 >> arg3;
                float result = SinIntegral(arg1, arg2, arg3);
                std::cout << "Result of integral = " << result << std::endl;
            } else if (command == "2") {
                std::cout << "PrimeCount function:" << std::endl;
                int arg1, arg2;
                std::cin >> arg1 >> arg2;
                int result = PrimeCount(arg1, arg2);
                std::cout << "Count of prime numbers = " << result << std::endl;
            } else {
                std::cout << "Invalid command" << std::endl;
            }
        }
    }
    UnloadLibrary(libraryHandle);
    return 0;
}