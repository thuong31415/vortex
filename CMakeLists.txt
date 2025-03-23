cmake_minimum_required(VERSION 3.28)
project(vortex)

set(CMAKE_CXX_STANDARD 20)

add_executable(vortex src/main.cpp
        src/network/Socket.cpp
        src/network/Socket.h
        src/server/Server.cpp
        src/server/Server.h
        src/http/HttpHandler.cpp
        src/http/HttpHandler.h
        src/http/HttpRequest.cpp
        src/http/HttpRequest.h
        src/http/HttpResponse.cpp
        src/http/HttpResponse.h
)
