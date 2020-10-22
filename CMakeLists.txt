cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME cloud_client)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")

add_definitions(-std=c++17)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include/)

list (APPEND SOURCES 
    ${SRC_DIR}/main.cpp
)

add_executable(cloud_client ${SOURCES})
