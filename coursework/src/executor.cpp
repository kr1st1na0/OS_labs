#include "executor.hpp"

int StartProcess(const std::string &path) {
    int pid = fork();
    if (pid == -1) {
        throw std::logic_error("Can't fork");
    }
    else if (pid == 0) {
        if (execl(path.c_str(), path.c_str(), nullptr) == -1) {
            throw std::logic_error("Can't exec");
        } 
    } else {
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
    return 0;
}

void Pipe::Push(size_t id) {
    {
        std::lock_guard<std::mutex> lk(mtx);
        q.push(id);
    }
    cv.notify_one();
}

std::vector<size_t> Pipe::Pop() {
    std::vector<size_t> result;
    {
        std::unique_lock<std::mutex> lk(mtx);
        if (q.empty()) {
            cv.wait(lk);
        }
        while (!q.empty()) {
            result.push_back(q.front());
            q.pop();
        }
    }
    return result;
}

Executor::Executor(DAG &_dag) : dag(_dag) { }

void Executor::ExecuteJob(size_t id, Job job, Pipe *pipe) {
    int result = StartProcess(job.path);
    if (result != 0) {
        exit(EXIT_FAILURE);
    } else {
        pipe->Push(id);
    }
}

void Executor::Execute(size_t threadCount) {
    freeThreads = threadCount;
    size_t count = dag.GetJobs().size();
    size_t iter = count;
    std::vector<size_t> toExecute;
    Pipe pipe;
    std::vector<int> numOfDeps(count, 0);

    for (size_t from = 0; from < count; ++from) {
        for (const auto &to : dag.GetGraph().GetEdges()[from]) {
            numOfDeps[to]++;
        }
    }

    for (size_t id = 0; id < count; ++id) {
        if (numOfDeps[id] == 0) {
            toExecute.push_back(id);
            numOfDeps[id] = -1;
        }
    }

    while (iter != 0) {
        while (!toExecute.empty() && freeThreads != 0) {
            size_t id = toExecute[toExecute.size() - 1];
            std::thread t(&Executor::ExecuteJob, this, id, dag.GetJobs()[id], &pipe);
            t.detach();
            freeThreads--;
            toExecute.pop_back();
        }

        std::vector<size_t> result = pipe.Pop();
        for (const auto &id : result) {
            freeThreads++;
            iter--;
            for (const auto &to : dag.GetGraph().GetEdges()[id]) {
                numOfDeps[to]--;
            }
        }

        for (size_t id = 0; id < count; ++id) {
            if (numOfDeps[id] == 0) {
                toExecute.push_back(id);
                numOfDeps[id] = -1;
            }
        }
    }
}

