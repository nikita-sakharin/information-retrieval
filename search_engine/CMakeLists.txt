cmake_minimum_required(VERSION 3.14.0)

project(search_engine VERSION 1.0.0 LANGUAGES CXX)

if (CYGWIN)
    set(CMAKE_AR "/usr/bin/gcc-ar.exe")
endif()

if (UNIX)
    add_compile_options(-Wall -Wcast-align -Wcast-qual -Wdouble-promotion
        -Weffc++ -Werror -Wextra -Wfatal-errors -Wfloat-equal -Wformat=2
        -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2
        -Wimplicit-fallthrough=5 -Winit-self -Winline -Wmain -Wmissing-braces
        -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference
        -Wpadded -Wpedantic -Wredundant-decls -Wshadow=global -Wswitch-default
        -Wswitch-enum -Wundef -Wunreachable-code -Wunused-const-variable=2
        -Wunused-macros -Wzero-as-null-pointer-constant -pedantic-errors
    )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/googletest)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME test COMMAND ${PROJECT_NAME}_test)
