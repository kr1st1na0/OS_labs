#include <iostream>
#include <fstream>
#include <chrono>

#include "inipp.h"

#include "parser.hpp"
#include "executor.hpp"

// bash: export PATH_TO_CONFIG="/home/kristinab/ubuntu_main/OS_labs/coursework/data/ex1/config.ini"

int main(int argc, char ** argv) {
    size_t threadCount = 4;
    if (argc > 1) {
        threadCount = std::atoi(argv[1]);
    }
    
    DAG dag = Parse(std::string(getenv("PATH_TO_CONFIG")));
    Executor exec(dag);

    // auto begin = std::chrono::high_resolution_clock::now();
    exec.Execute(threadCount);
    // auto end = std::chrono::high_resolution_clock::now();
    // int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    // std::cout << "Time for " << threadCount << " threads: " << time << std::endl;
    return 0;
}