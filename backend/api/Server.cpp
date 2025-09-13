#include "Server.h"

using namespace web;
using namespace http;
using namespace web::http::experimental::listener;

Server::Server(Scheduler& sched, MemoryManager& mem, DeadlockDetector& dd,
               DiskScheduler& dsk, PageReplacement& pr,
               const utility::string_t& url)
    : listener(url), scheduler(sched), memory(mem), deadlock(dd), disk(dsk), pager(pr), running(false) {

    // CORS preflight
    listener.support(methods::OPTIONS, [](http_request request) {
        http_response res(status_codes::OK);
        res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        res.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
        res.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
        request.reply(res);
    });

    // POST routes
    listener.support(methods::POST, [this](http_request request) {
        auto path = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!path.empty()) {
            if (path[0] == U("disk")) handleDisk(request);
            else if (path[0] == U("pages")) handlePages(request);
            else if (path[0] == U("memory")) handleMemory(request);
            else handlePost(request);  // default → add process
        }
    });

    // GET routes
    listener.support(methods::GET, [this](http_request request) {
        auto path = uri::split_path(uri::decode(request.relative_uri().path()));
        if (!path.empty()) {
            if (path[0] == U("disk")) handleDisk(request);
            else if (path[0] == U("pages")) handlePages(request);
            else if (path[0] == U("memory")) handleMemory(request);
            else if (path[0] == U("deadlock")) handleDeadlock(request);
            else handleGet(request);   // default → scheduler + memory
        }
    });
}

void Server::start() {
    listener.open().wait();
    running = true;
    simulationThread = std::thread(&Server::simulationLoop, this);
    std::cout << "Server running at " << utility::conversions::to_utf8string(listener.uri().to_string()) << std::endl;
}

void Server::stop() {
    running = false;
    if (simulationThread.joinable()) simulationThread.join();
    listener.close().wait();
}

// ---------------- CPU Scheduling ----------------
void Server::handlePost(http_request request) {
    request.extract_json().then([=](json::value body) {
        int pid = body[U("pid")].as_integer();
        std::string name = utility::conversions::to_utf8string(body[U("name")].as_string());
        int arrival = body[U("arrival")].as_integer();
        int burst = body[U("burst")].as_integer();
        int priority = body[U("priority")].as_integer();

        Process* p = new Process(pid, name, arrival, burst, priority);
        scheduler.addProcess(p);

        json::value response;
        response[U("status")] = json::value::string(U("process added"));

        http_response res(status_codes::OK);
        res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        res.set_body(response);
        request.reply(res);
    }).wait();
}

void Server::handleGet(http_request request) {
    json::value response;

    // Scheduler
    json::value procs = json::value::array();
    auto processes = scheduler.getState();
    int i = 0;
    for (auto* p : processes) {
        json::value proc;
        proc[U("pid")] = json::value::number(p->pid);
        proc[U("name")] = json::value::string(utility::conversions::to_string_t(p->name));
        proc[U("remaining")] = json::value::number(p->remainingTime);
        proc[U("priority")] = json::value::number(p->priority);
        proc[U("state")] = json::value::string(
            utility::conversions::to_string_t(std::to_string(p->state))
        );
        procs[i++] = proc;
    }
    response[U("processes")] = procs;

    // Memory
    json::value memState = json::value::array();
    auto blocks = memory.getState();
    int j = 0;
    for (auto& b : blocks) {
        json::value blk;
        blk[U("start")] = json::value::number(b.start);
        blk[U("size")] = json::value::number(b.size);
        blk[U("owner")] = json::value::string(utility::conversions::to_string_t(b.owner));
        blk[U("allocated")] = json::value::boolean(b.allocated);
        memState[j++] = blk;
    }
    response[U("memory")] = memState;

    http_response res(status_codes::OK);
    res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    res.set_body(response);
    request.reply(res);
}

// ---------------- Memory ----------------
void Server::handleMemory(http_request request) {
    if (request.method() == methods::POST) {
        request.extract_json().then([=](json::value body) {
            std::string pid = utility::conversions::to_utf8string(body[U("pid")].as_string());
            int size = body[U("size")].as_integer();
            bool success = memory.allocate(pid, size);

            json::value resJson;
            resJson[U("success")] = json::value::boolean(success);

            http_response res(status_codes::OK);
            res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            res.set_body(resJson);
            request.reply(res);
        }).wait();
    } else {
        json::value memState = json::value::array();
        auto blocks = memory.getState();
        int j = 0;
        for (auto& b : blocks) {
            json::value blk;
            blk[U("start")] = json::value::number(b.start);
            blk[U("size")] = json::value::number(b.size);
            blk[U("owner")] = json::value::string(utility::conversions::to_string_t(b.owner));
            blk[U("allocated")] = json::value::boolean(b.allocated);
            memState[j++] = blk;
        }
        http_response res(status_codes::OK);
        res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        res.set_body(memState);
        request.reply(res);
    }
}

// ---------------- Disk ----------------
void Server::handleDisk(http_request request) {
    if (request.method() == methods::POST) {
        request.extract_json().then([=](json::value body) {
            int track = body[U("track")].as_integer();
            disk.addRequest(track);

            json::value response;
            response[U("status")] = json::value::string(U("disk request added"));

            http_response res(status_codes::OK);
            res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            res.set_body(response);
            request.reply(res);
        }).wait();
    } else if (request.method() == methods::GET) {
        json::value resJson;
        resJson[U("head")] = json::value::number(disk.nextMove());

        auto pending = disk.getState();
        json::value arr = json::value::array();
        int i = 0;
        for (auto t : pending) arr[i++] = json::value::number(t);

        resJson[U("queue")] = arr;

        http_response res(status_codes::OK);
        res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        res.set_body(resJson);
        request.reply(res);
    }
}

// ---------------- Pages ----------------
void Server::handlePages(http_request request) {
    if (request.method() == methods::POST) {
        request.extract_json().then([=](json::value body) {
            int page = body[U("page")].as_integer();
            int time = (int)std::time(nullptr);
            bool hit = pager.accessPage(page, time);

            json::value response;
            response[U("hit")] = json::value::boolean(hit);

            http_response res(status_codes::OK);
            res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            res.set_body(response);
            request.reply(res);
        }).wait();
    } else if (request.method() == methods::GET) {
        auto pages = pager.getState();
        json::value arr = json::value::array();
        int i = 0;
        for (auto p : pages) arr[i++] = json::value::number(p);

        json::value response;
        response[U("pages")] = arr;

        http_response res(status_codes::OK);
        res.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        res.set_body(response);
        request.reply(res);
    }
}

// ---------------- Deadlock ----------------
void Server::handleDeadlock(http_request request) {
    json::value res;
    std::map<std::string, std::vector<std::string>> graph; // TODO: populate with real data
    bool deadlocked = deadlock.detectCycle(graph);

    res[U("deadlock")] = json::value::boolean(deadlocked);

    http_response httpRes(status_codes::OK);
    httpRes.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    httpRes.set_body(res);
    request.reply(httpRes);
}

// ---------------- Loop ----------------
void Server::simulationLoop() {
    while (running) {
        scheduler.tick();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
