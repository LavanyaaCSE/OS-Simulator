cmake_minimum_required(VERSION 3.15)
project(OS_Simulation_Backend)

set(CMAKE_CXX_STANDARD 17)

# Collect only .cpp files (not .h!)
set(SOURCES
    main.cpp
    core/DeadlockDetector.cpp
    core/DiskSchedular.cpp
    core/MemoryManager.cpp
    core/PageReplacement.cpp
    core/Schedular.cpp
    api/Server.cpp
)

add_executable(OS_Simulation_Backend ${SOURCES})

# Use cpprestsdk from vcpkg
find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(OS_Simulation_Backend PRIVATE cpprestsdk::cpprest)
