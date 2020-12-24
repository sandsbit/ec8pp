cmake_minimum_required(VERSION 3.17)
project(ec8++ VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(lib/glad)

find_package(glfw3 3.3 REQUIRED)

add_executable(ec8pp ec8++/main.cpp ec8++/graphics.cpp ec8++/graphics.h)
target_link_libraries(ec8pp PUBLIC glfw glad)
target_link_libraries(ec8pp PRIVATE Threads::Threads)