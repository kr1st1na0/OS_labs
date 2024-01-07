#include "dag.hpp"

DAG::DAG(const std::vector<Job> &_jobs, const Graph &_graph) : jobs(_jobs), graph(_graph) {
    if (graph.NodeCount() != jobs.size()) {
        throw std::logic_error("Nodes count != jobs count");
    }
    if (!graph.CheckCycles()) {
        throw std::logic_error("Graph has cycle");
    }
}

const std::vector<Job> &DAG::GetJobs() const {
    return jobs;
}

const Graph &DAG::GetGraph() const {
    return graph;
}
