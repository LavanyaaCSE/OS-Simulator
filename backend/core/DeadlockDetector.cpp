#include "DeadlockDetector.h"
#include <set>

bool dfs(const std::string& node,
         std::map<std::string, std::vector<std::string>> graph,
         std::set<std::string>& visited,
         std::set<std::string>& recStack) {
    if (!visited.count(node)) {
        visited.insert(node);
        recStack.insert(node);

        for (auto &nbr : graph[node]) {
            if (!visited.count(nbr) && dfs(nbr, graph, visited, recStack)) return true;
            else if (recStack.count(nbr)) return true;
        }
    }
    recStack.erase(node);
    return false;
}

bool DeadlockDetector::detectCycle(const std::map<std::string, std::vector<std::string>>& graph) {
    std::set<std::string> visited, recStack;
    for (auto &kv : graph) {
        if (dfs(kv.first, graph, visited, recStack)) return true;
    }
    return false;
}
