cmake_minimum_required(VERSION 3.27)
project(Algo_N3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

add_executable(Algo_N3 main.cpp)
