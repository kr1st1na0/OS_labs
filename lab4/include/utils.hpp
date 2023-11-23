#include <iostream>
#include <cstdlib>
#include <dlfcn.h>

using SinIntegralFunc = float (*)(float, float, float);
using PrimeCountFunc = int (*)(int, int);

void* LoadLibrary(const char *libraryName);
void UnloadLibrary(void* handle);