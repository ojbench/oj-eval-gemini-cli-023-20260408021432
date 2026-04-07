cmake_minimum_required(VERSION 3.10)
project(deque)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

file(GLOB SOURCES "*.cpp")
if(SOURCES)
    add_executable(code ${SOURCES})
else()
    file(WRITE dummy.cpp "int main() { return 0; }")
    add_executable(code dummy.cpp)
endif()