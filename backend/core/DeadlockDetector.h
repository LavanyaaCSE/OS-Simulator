#ifndef DEADLOCK_H
#define DEADLOCK_H

#include <map>
#include <vector>
#include <string>

class DeadlockDetector {
public:
    bool detectCycle(const std::map<std::string, std::vector<std::string>>& graph);
};

#endif
