#include "graph.hpp"

bool Graph::Dfs(Node current, std::vector<int> &visited) const {
    visited[current] = 1;
    for (const auto& to : edges[current]) {
        if (visited[to] == 1) {
            return true;
        } else if (visited[to] == 0) {
            bool result = Dfs(to, visited);
            if (result) {
                return true;
            }
        }
    }
    visited[current] = 2;
    return false;
}

size_t Graph::NodeCount() const {
    return edges.size();
}

void Graph::AddEdge(Node from, Node to) {
    edges[from].push_back(to);
}

bool Graph::CheckCycles() const {
    std::vector<int> visited(NodeCount(), 0);
    for (Node node = 0; node < NodeCount(); ++node) {
        if (visited[node] == 0) {
            if (Dfs(node, visited)) {
                return false;
            }
        }
    }
    return true;
}

const std::vector<std::vector<Graph::Node> > &Graph::GetEdges() const {
    return edges;
}
