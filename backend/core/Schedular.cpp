#include "Scheduler.h"
#include <algorithm>

Scheduler::Scheduler(std::string algo, int q) : currentTime(0), algorithm(algo), quantum(q) {}

void Scheduler::addProcess(Process* p) {
    std::lock_guard<std::mutex> lock(mtx);
    processes.push_back(p);
}

void Scheduler::tick() {
    std::lock_guard<std::mutex> lock(mtx);

    if (processes.empty()) { currentTime++; return; }

    // Example: FCFS
    auto* p = processes.front();
    if (p->state != RUNNING) p->state = RUNNING;
    p->remainingTime--;
    currentTime++;

    if (p->remainingTime == 0) {
        p->state = TERMINATED;
        processes.erase(processes.begin());
    }
}

std::vector<Process*> Scheduler::getState() {
    std::lock_guard<std::mutex> lock(mtx);
    return processes;
}
