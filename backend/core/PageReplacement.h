#ifndef PAGEREPLACEMENT_H
#define PAGEREPLACEMENT_H

#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>

class PageReplacement {
private:
    int capacity;
    std::string algorithm;
    std::queue<int> fifoQueue;
    std::unordered_set<int> pages;
    std::unordered_map<int, int> lruMap; // page → last used

public:
    PageReplacement(int cap, std::string algo="FIFO");
    bool accessPage(int page, int time);
    std::vector<int> getState();
};

#endif
