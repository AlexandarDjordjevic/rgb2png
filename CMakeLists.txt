cmake_minimum_required(VERSION 3.8)

project(raw2png)

set(target raw2png)

set(CMAKE_CXX_STANDARD 20)

add_executable(${target} main.cpp src/PngImage.cpp)

target_link_libraries(${target} PRIVATE png)
target_include_directories(${target} PRIVATE include)
