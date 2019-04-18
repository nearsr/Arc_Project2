cmake_minimum_required(VERSION 3.12)
project(CacheSim)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CacheSim
        ._Makefile
        ._pinatrace.out
        cache.cpp
        cache.h
        main.cpp
        Makefile
        misc.h
        pinatrace.out
        prefetch.cpp
        prefetch.h
        system.cpp
        system.h)
