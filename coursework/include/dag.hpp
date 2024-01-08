#pragma once

#include <iostream>
#include <vector>

#include "graph.hpp"

struct Job {
    std::string name, path;
};

class DAG {
private:
    std::vector<Job> jobs;
    Graph graph;

public:
    DAG() = delete;
    DAG(const std::vector<Job> &_jobs, const Graph &_graph);

    const std::vector<Job> &GetJobs() const;
    const Graph &GetGraph() const;
};
