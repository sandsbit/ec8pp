cmake_minimum_required(VERSION 3.17)
project(ec8++ VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(ec8pp ec8++/main.cpp)
target_include_directories(ec8pp PRIVATE ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(ec8pp ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )