#include "utils.hpp"

void* LoadLibrary(const char *libraryName) {
    void* handle = dlopen(libraryName, RTLD_LAZY);
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