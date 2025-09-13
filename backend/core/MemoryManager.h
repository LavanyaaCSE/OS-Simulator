#pragma once 

#include <vector>
#include <string>

struct Block {
    int start;
    int size;
    std::string owner;
    bool allocated;
};

class MemoryManager {
private:
    std::vector<Block> memory;

public:
    explicit MemoryManager(int totalSize);

    bool allocate(const std::string& pid, int size);
    void free(const std::string& pid);
    std::vector<Block> getState() const;
};
