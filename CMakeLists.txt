cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME cloud_client)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall")

add_definitions(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

set(LIBS ${CMAKE_SOURCE_DIR}/lib)
set(WOLK_SDK ${LIBS}/WolkConnect-Cpp)
set(WOLK_SDK_LIB ${WOLK_SDK}/out/lib)
set(GATTLIB_ROOT ${LIBS}/gattlib)
set(GATTLIB_LIB ${GATTLIB_ROOT}/build/dbus/)
set(GATTLIB_INCLUDE ${GATTLIB_ROOT}/include)
set(GATTLIBPP_ROOT ${LIBS}/gattlibpp)
set(GATTLIBPP_LIB ${GATTLIBPP_ROOT}/build/gattlibpp)
set(LIBBLEPP ${LIBS}/libblepp)

include_directories(
${WOLK_SDK}/src
${WOLK_SDK}/WolkSDK-Cpp/core
${GATTLIBPP_ROOT}/gattlibpp
${GATTLIBPP_ROOT}/include
${LIBBLEPP}
${GATTLIB_INCLUDE}
)

# Wolk
add_library(WOLKABOUT_CONNECTOR SHARED IMPORTED)
set_target_properties(WOLKABOUT_CONNECTOR PROPERTIES IMPORTED_LOCATION ${WOLK_SDK_LIB}/libWolkAboutConnector.so)
add_library(WOLKABOUT_CORE SHARED IMPORTED)
set_target_properties(WOLKABOUT_CORE PROPERTIES IMPORTED_LOCATION ${WOLK_SDK_LIB}/libWolkAboutCore.so)

# GATTLIB
add_library(GATTLIB SHARED IMPORTED)
set_target_properties(GATTLIB PROPERTIES IMPORTED_LOCATION ${GATTLIB_LIB}/libgattlib.so)

# GATTLIBPP
add_library(GATTLIBPP SHARED IMPORTED)
set_target_properties(GATTLIBPP PROPERTIES IMPORTED_LOCATION ${GATTLIBPP_LIB}/libgattlibpp.so)

# LIBBLEPP
add_library(LIBBLEPP SHARED IMPORTED)
set_target_properties(LIBBLEPP PROPERTIES IMPORTED_LOCATION ${LIBBLEPP}/libble++.so)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

list (APPEND SOURCES
    ${SRC_DIR}/main.cpp
)

add_executable(cloud_client ${SOURCES})
target_link_libraries(cloud_client PRIVATE 
GATTLIB 
GATTLIBPP
#LIBBLEPP
WOLKABOUT_CONNECTOR 
WOLKABOUT_CORE)
