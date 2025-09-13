#include "DiskScheduler.h"
#include <algorithm>
#include <cmath>

DiskScheduler::DiskScheduler(int start, std::string algo) : head(start), algorithm(algo) {}

void DiskScheduler::addRequest(int track) {
    requests.push_back(track);
}

int DiskScheduler::nextMove() {
    if (requests.empty()) return head;

    if (algorithm == "FCFS") {
        int next = requests.front();
        requests.erase(requests.begin());
        head = next;
        return head;
    }
    else if (algorithm == "SSTF") {
        auto it = std::min_element(requests.begin(), requests.end(),
            [&](int a, int b) { return std::abs(a-head) < std::abs(b-head); });
        int next = *it;
        requests.erase(it);
        head = next;
        return head;
    }
    // TODO: SCAN, LOOK
    return head;
}

std::vector<int> DiskScheduler::getState() {
    return requests;
}
