#ifndef SERVER_H
#define SERVER_H

#include "../core/Scheduler.h"
#include "../core/MemoryManager.h"
#include "../core/DeadlockDetector.h"
#include "../core/DiskScheduler.h"
#include "../core/PageReplacement.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <thread>
#include <atomic>

using namespace web;
using namespace http;
using namespace web::http::experimental::listener;

class Server {
private:
    http_listener listener;
    Scheduler& scheduler;
    MemoryManager& memory;
    DeadlockDetector& deadlock;
    DiskScheduler& disk;
    PageReplacement& pager;
    std::atomic<bool> running;
    std::thread simulationThread;

public:
    Server(Scheduler& sched, MemoryManager& mem, DeadlockDetector& dd,
           DiskScheduler& dsk, PageReplacement& pr,
           const utility::string_t& url);
    void start();
    void stop();

private:
    void handlePost(http_request request);
    void handleGet(http_request request);
    void handleDisk(http_request request);
    void handlePages(http_request request);
    void handleMemory(http_request request);
    void handleDeadlock(http_request request);
    void simulationLoop();
};

#endif
