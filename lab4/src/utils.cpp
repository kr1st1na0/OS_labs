#include "utils.hpp"

void* LoadLibrary(const std::string& libraryName) {
    void* handle = dlopen(libraryName.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Couldn't load the library: " << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }
    return handle;
}

void UnloadLibrary(void* handle) {
    if (dlclose(handle) != 0) {
        std::cerr << "Couldn't unload the library: " << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }
}