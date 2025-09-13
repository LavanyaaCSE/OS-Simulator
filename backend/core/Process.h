#ifndef PROCESS_H
#define PROCESS_H

#include <string>

enum State { NEW, READY, RUNNING, WAITING, TERMINATED };

struct Process {
    int pid;
    std::string name;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    State state;

    Process(int id, std::string n, int arrival, int burst, int prio)
        : pid(id), name(n), arrivalTime(arrival), burstTime(burst),
          remainingTime(burst), priority(prio), state(NEW) {}
};

#endif
