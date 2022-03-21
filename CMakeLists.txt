cmake_minimum_required(VERSION 3.5)

project(spiral_galaxies LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(spiral_galaxies main.cpp spiral_galaxies.cpp spiral_galaxies.h)
