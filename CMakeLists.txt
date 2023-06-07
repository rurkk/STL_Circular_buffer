cmake_minimum_required(VERSION 3.23)
project(labwork_8_rurkk)

set(CMAKE_CXX_STANDARD 23)

add_executable(labwork_8_rurkk
        main.cpp
        CCirtucalBuffer.h
        CCirtucalBufferExt.h)

enable_testing()
add_subdirectory(tests)
