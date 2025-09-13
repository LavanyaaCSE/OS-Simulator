#ifndef DISKSCHEDULER_H
#define DISKSCHEDULER_H

#include <vector>
#include <string>

class DiskScheduler {
private:
    int head;
    std::string algorithm;
    std::vector<int> requests;

public:
    DiskScheduler(int start, std::string algo);
    void addRequest(int track);
    int nextMove();                       // consumes next request
    int getHead() const { return head; }  // just returns current head
    std::vector<int> getState();
};

#endif
