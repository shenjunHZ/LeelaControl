# CMakeList.txt
# Copyright (C) 2019 jun shen
#
cmake_minimum_required (VERSION 3.1)

project ("LeelaControl")
set(EXECUTABLE_NAME "LeelaControl")

if(${CMAKE_C_COMPILER} MATCHES ".*Microsoft.*" )
    set(WIN_ENV TRUE)
    message(STATUS "==windows==")
elseif ($ENV{TARGET} MATCHES "arm-linux-gnueabihf")
    set(ARM_LINUX TRUE)
    add_definitions(-DRUN_IN_ARM)
    message(STATUS "==ARM LINUX==")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lboost_thread")

set(USE_CPU_ONLY TRUE)

if($ENV{TARGET} MATCHES "arm-linux-gnueabihf")
    add_definitions(-DARM_CPU_RUN)
endif()

if(USE_CPU_ONLY)
    add_definitions(-DUSE_CPU_ONLY)
    message(STATUS "==CPU ONLY==")
endif()

MESSAGE(STATUS "LeelaControl")
add_subdirectory ("LeelaControl")


