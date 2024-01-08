#pragma once

#include <iostream>
#include <vector>
#include <map>

class Graph {
public:
    using Node = size_t;

    Graph(size_t N) : edges(N) { }

    size_t NodeCount() const;
    void AddEdge(Node from, Node to);
    bool CheckCycles() const;

    const std::vector<std::vector<Node> > &GetEdges() const;
private:
    std::vector<std::vector<Node> > edges;

    bool Dfs(Node current, std::vector<int> &visited) const;
};
