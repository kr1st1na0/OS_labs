#pragma once

#include <thread>
#include <queue>
#include <set>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unistd.h>
#include <wait.h>

#include "dag.hpp"

int StartProcess(const std::string &path);

class Pipe {
private:
    std::queue<size_t> q;
    std::mutex mtx;
    std::condition_variable cv;
public:
    void Push(size_t);
    std::vector<size_t> Pop();
};

class Executor {
private:
    DAG &dag;
    size_t freeThreads;

    void ExecuteJob(size_t id, Job job, Pipe *pipe);
public:
    Executor(DAG &_dag);
    void Execute(size_t threadCount);

};