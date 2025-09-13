#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <queue>
#include <vector>
#include <mutex>
#include <string>

class Scheduler {
private:
    std::vector<Process*> processes;
    int currentTime;
    std::string algorithm;
    int quantum;
    std::mutex mtx;

public:
    Scheduler(std::string algo="FCFS", int q=4);
    void addProcess(Process* p);
    void tick();
    std::vector<Process*> getState();
};

#endif
