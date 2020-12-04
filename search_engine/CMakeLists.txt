cmake_minimum_required(VERSION 3.14.0)

project(search_engine VERSION 1.0.0 LANGUAGES CXX)

if (CYGWIN)
    set(CMAKE_AR "/usr/bin/gcc-ar.exe")
endif()

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Wall -Werror -Wextra -Wfatal-errors -Wpedantic -pedantic-errors"
    )
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    # -flto -s
    if (CMAKE_BUILD_TYPE EQUAL RELEASE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/googletest)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME test COMMAND ${PROJECT_NAME}_test)
