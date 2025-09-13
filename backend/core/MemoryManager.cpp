#include "MemoryManager.h"

MemoryManager::MemoryManager(int totalSize) {
    memory.push_back({0, totalSize, "", false});
}

bool MemoryManager::allocate(const std::string& pid, int size) {
    for (auto &block : memory) {
        if (!block.allocated && block.size >= size) {
            block.owner = pid;
            block.allocated = true;
            return true;
        }
    }
    return false;
}

void MemoryManager::free(const std::string& pid) {
    for (auto &block : memory) {
        if (block.owner == pid) {
            block.owner = "";
            block.allocated = false;
        }
    }
}

std::vector<Block> MemoryManager::getState() const {
    return memory;
}
