cmake_minimum_required(VERSION 3.16)
project(memorylink_api)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE NETWORK_SOURCE src/utils/*.cpp)
file(GLOB_RECURSE UTILS_SOURCE src/network/*.cpp)

set(SOURCES
        src/main.cpp
        ${NETWORK_SOURCE}
        ${UTILS_SOURCE}
)

add_executable(memorylink_api ${SOURCES})

target_include_directories(memorylink_api PRIVATE
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_SOURCE_DIR}/src
)
