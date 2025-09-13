#include "core/Scheduler.h"
#include "core/MemoryManager.h"
#include "core/DeadlockDetector.h"
#include "core/DiskScheduler.h"
#include "core/PageReplacement.h"
#include "api/Server.h"
#include <iostream>

int main() {
    // Initialize OS modules
    Scheduler scheduler("FCFS", 4);         // CPU Scheduler
    MemoryManager memory(256);              // Memory size = 256 units
    DeadlockDetector deadlock;              // Deadlock detection
    DiskScheduler disk(50, "SSTF");         // Disk scheduling starting at track 50
    PageReplacement pager(3, "LRU");        // 3-frame memory, LRU algorithm

    // Start REST server
    utility::string_t address = U("http://localhost:8080");
    Server server(scheduler, memory, deadlock, disk, pager, address);
    server.start();

    std::cout << "Press ENTER to stop the server..." << std::endl;
    std::cin.get();

    server.stop();
    return 0;
}
