cmake_minimum_required(VERSION 3.10)

project(SomeGraphics VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(SomeGraphics ${SRCS})
