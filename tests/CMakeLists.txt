cmake_minimum_required(VERSION 3.8)
project(ec8pp-tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ec8pp_tests main.cpp)

target_include_directories(ec8pp_tests PUBLIC ../lib ../ec8++)