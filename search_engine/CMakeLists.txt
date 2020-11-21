cmake_minimum_required(VERSION 3.6)

project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main main.cpp header.hpp)
# add_subdirectory(subLibrary subLibrary/build)
# add_subdirectory(# test)
