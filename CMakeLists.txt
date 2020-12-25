cmake_minimum_required(VERSION 3.17)
project(ec8++ VERSION 1.0 LANGUAGES C CXX OBJC)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(lib/glad)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenAL REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(ec8pp ec8++/main.cpp ec8++/graphics.cpp ec8++/graphics.h ec8++/openal.h ec8++/timers.cpp ec8++/timers.h ec8++/emulator.cpp ec8++/emulator.h ec8++/dialog.h ec8++/input.cpp ec8++/input.h)
target_link_libraries(ec8pp PUBLIC glfw glad openal sdl2)
target_link_libraries(ec8pp PRIVATE Threads::Threads)

target_sources(ec8pp PRIVATE ec8++/dialog.cpp)


#if (APPLE)
#    find_library(FrameworkFoundation Foundation)
#    if (NOT FrameworkFoundation)
#        message(FATAL_ERROR "Foundation framework not found")
#    endif()
#
#    find_library(FrameworkCocoa Cocoa)
#    if (NOT FrameworkCocoa)
#        message(FATAL_ERROR "Cocoa framework not found")
#    endif()
#
#    find_library(FrameworkOpenAL OpenAL)
#    if (NOT FrameworkOpenAL)
#        message(FATAL_ERROR "Cocoa framework not found")
#    endif()
#
#    target_link_libraries(ec8pp PUBLIC ${FrameworkFoundation} ${FrameworkCocoa} ${FrameworkOpenAL})
#    target_sources(ec8pp PRIVATE ec8++/dialog.m)
#else()
#    target_sources(ec8pp PRIVATE ec8++/dialog.cpp)
#endif()