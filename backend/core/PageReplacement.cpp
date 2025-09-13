#include "PageReplacement.h"
#include <algorithm>
#include <string>

PageReplacement::PageReplacement(int cap, std::string algo)
    : capacity(cap), algorithm(algo) {}

bool PageReplacement::accessPage(int page, int time) {
    // Hit
    if (pages.count(page)) {
        if (algorithm == "LRU") lruMap[page] = time;
        return true;
    }

    // Miss
    if ((int)pages.size() < capacity) {
        pages.insert(page);
        fifoQueue.push(page);
        lruMap[page] = time;
    } else {
        if (algorithm == "FIFO") {
            int victim = fifoQueue.front(); fifoQueue.pop();
            pages.erase(victim);
            pages.insert(page);
            fifoQueue.push(page);
        } else if (algorithm == "LRU") {
            auto victim = std::min_element(lruMap.begin(), lruMap.end(),
                [](auto &a, auto &b){ return a.second < b.second; });
            pages.erase(victim->first);
            lruMap.erase(victim->first);
            pages.insert(page);
            lruMap[page] = time;
        }
    }
    return false;
}

std::vector<int> PageReplacement::getState() {
    return std::vector<int>(pages.begin(), pages.end());
}
