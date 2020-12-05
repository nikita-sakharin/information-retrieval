set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BUILD_TYPE STREQUAL RELEASE)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Wcast-align -Wcast-qual -Wdouble-promotion -Weffc++ -Wfloat-equal \
        -Wformat=2 -Wformat-overflow=2 -Wformat-signedness \
        -Wformat-truncation=2 -Wimplicit-fallthrough=5 -Winit-self -Winline \
        -Wmain -Wmissing-braces -Wmissing-declarations -Wmissing-include-dirs \
        -Wnull-dereference -Wpadded -Wredundant-decls -Wshadow=global \
        -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code \
        -Wunused-const-variable=2 -Wunused-macros \
        -Wzero-as-null-pointer-constant"
    )
endif()

set(BINARY ${PROJECT_NAME})

add_executable(${BINARY} main.cpp tokenizator.cpp)
target_include_directories(${BINARY} PRIVATE "${PROJECT_SOURCE_DIR}/include")
