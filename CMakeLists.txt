cmake_minimum_required(VERSION 3.28)
project(vortex)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SERVER_SOURCES
        src/main.cpp
        src/network/*.cpp
        src/server/*.cpp
        src/http/*.cpp
        src/thread_pool/*.cpp
)

file(GLOB_RECURSE CLIENT_SOURCES
        src/client/*.cpp
)

add_executable(vortex ${SERVER_SOURCES})


add_executable(vortex_client ${CLIENT_SOURCES})