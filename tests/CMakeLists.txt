cmake_minimum_required(VERSION 3.8)
project(ec8pp-tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(ec8pp_tests main.cpp ec8++/emulator.cpp ec8++/emulator_functions.h ../ec8++/emulator.cpp
        ../ec8++/timers.cpp ../ec8++/input.cpp ../ec8++/graphics.cpp)

if (APPLE)
    find_library(FrameworkOpenAL OpenAL)
    if (NOT FrameworkOpenAL)
        message(FATAL_ERROR "OpenAL framework not found")
    endif()

    target_link_libraries(ec8pp_tests PUBLIC ${FrameworkOpenAL})
endif()

target_link_libraries(ec8pp_tests PUBLIC glfw glad openal)
target_link_libraries(ec8pp_tests PRIVATE Threads::Threads)

target_include_directories(ec8pp_tests PUBLIC ../lib ../ec8++)