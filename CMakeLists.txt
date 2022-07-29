cmake_minimum_required(VERSION 3.22)
project(Carl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS -static)
set(json.hpp)
set(whereami.hpp)

add_executable(Carl main.cpp whereami.cpp)
