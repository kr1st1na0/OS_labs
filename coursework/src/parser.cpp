#include "parser.hpp"
#include <iostream>

DAG Parse(const std::string &path) {
    inipp::Ini<char> ini;
    std::ifstream is(path);
    ini.parse(is);
    
    std::string pathToBin, rawJobs, rawDependencies, rawCount;
    size_t count;

    inipp::get_value(ini.sections["general"], "bin_path", pathToBin);
    inipp::get_value(ini.sections["jobs"], "count", rawCount);
    inipp::get_value(ini.sections["jobs"], "jobs", rawJobs);
    inipp::get_value(ini.sections["dependencies"], "dependencies", rawDependencies);

    count = std::stoi(rawCount);
    std::vector<Job> jobs;
    Graph graph(count);
    std::map<std::string, size_t> jobsToId;

    std::stringstream ss(rawJobs);
    std::string current;
    while (getline(ss, current, ',')) {
        std::string name(current.begin() + 1, current.end());
        getline(ss, current, ',');
        std::string path(current.begin(), current.end() - 1);
        path = pathToBin + "/" + path;
        jobs.push_back({name, path});
        jobsToId[name] = jobs.size() - 1;
    }

    ss = std::stringstream(rawDependencies);
    while (getline(ss, current, ',')) {
        std::string req(current.begin() + 1, current.end());
        getline(ss, current, ',');
        std::string target(current.begin(), current.end() - 1);
        graph.AddEdge(jobsToId[req], jobsToId[target]);
    }

    return DAG(jobs, graph);
}