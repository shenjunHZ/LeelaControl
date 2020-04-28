# CMakeList.txt
# Copyright (C) 2019 jun shen
#
cmake_minimum_required (VERSION 3.1)

project ("LeelaControl")
set(EXECUTABLE_NAME "LeelaControl")

MESSAGE(STATUS "Run in cxx compiler: ${CMAKE_CXX_COMPILER_ID}")

if(${CMAKE_C_COMPILER} MATCHES "MSVC" )
    set(WIN_ENV TRUE)
    add_definitions(-DWIN_ENV_RUN)
    set(CMAKE_CXX_FLAGS_RELEASE "/MD")
    #set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin)

    message(STATUS "==Windows environment==")

elseif ($ENV{TARGET} MATCHES "arm-linux-gnueabihf")
    set(ARM_LINUX TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lboost_thread -fPIC -pthread")
    add_definitions(-DRUN_IN_ARM)

    message(STATUS "==ARM LINUX==")

elseif($ENV{TARGET} MATCHES "gcc-linux")
    set(GCC_LINUX TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lboost_thread -fPIC -pthread")

    message(STATUS "==GCC LINUX==")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(ARM_LINUX true)
    add_definitions(-DRUN_IN_ARM)
    MESSAGE(STATUS "ARM env.")
endif()

set(USE_CPU_ONLY TRUE)

if($ENV{TARGET} MATCHES "arm-linux-gnueabihf")
    add_definitions(-DARM_CPU_RUN)
endif()

if(USE_CPU_ONLY)
    add_definitions(-DUSE_CPU_ONLY)
    message(STATUS "==CPU ONLY==")
endif()

MESSAGE(STATUS "LeelaControl")
add_subdirectory ("leela-control")


