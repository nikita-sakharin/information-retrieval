cmake_minimum_required(VERSION 3.14.0)

project(search_engine VERSION 1.0.0 LANGUAGES CXX)

if (CYGWIN)
    set(CMAKE_AR "/usr/bin/gcc-ar.exe")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS -Wall -Werror -Wextra -Wfatal-errors -Wpedantic -pedantic-errors)
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:${CXX_FLAGS}>")

add_subdirectory(lib/googletest)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME test COMMAND ${PROJECT_NAME}_test)
