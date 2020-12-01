cmake_minimum_required(VERSION 3.14.0)

project(search_engine VERSION 1.0.0 LANGUAGES CXX)

if (CYGWIN)
    set(CMAKE_AR "/usr/bin/gcc-ar.exe")
endif()

if (UNIX)
    add_compile_options(-Wall -Werror -Wextra -Wpedantic -pedantic-errors)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/googletest)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME test COMMAND ${PROJECT_NAME}_test)
