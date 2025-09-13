#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <string>

inline int getTimestamp() {
    return static_cast<int>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count()
    );
}

inline std::string stateToString(int state) {
    switch(state) {
        case 0: return "NEW";
        case 1: return "READY";
        case 2: return "RUNNING";
        case 3: return "WAITING";
        case 4: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

#endif
