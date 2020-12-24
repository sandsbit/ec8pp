cmake_minimum_required(VERSION 3.17)
project(ec8++ VERSION 1.0 LANGUAGES C CXX OBJC)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(lib/glad)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(ec8pp ec8++/main.cpp ec8++/graphics.cpp ec8++/graphics.h ec8++/openal.h ec8++/timers.cpp ec8++/timers.h ec8++/emulator.cpp ec8++/emulator.h ec8++/dialog.c ec8++/dialog.h)
target_link_libraries(ec8pp PUBLIC glfw glad opengl)
target_link_libraries(ec8pp PRIVATE Threads::Threads)

if (APPLE)
    find_library(FrameworkFoundation Foundation)
    if (NOT FrameworkFoundation)
        message(FATAL_ERROR "Foundation framework not found")
    endif()

    target_link_libraries(ec8pp PUBLIC ${FrameworkFoundation})
    target_sources(ec8pp PRIVATE ec8++/dialog.m)
endif()