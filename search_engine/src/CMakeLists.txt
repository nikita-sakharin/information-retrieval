set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

set(TARGET ${PROJECT_NAME})
set(CXXFLAGS -Wcast-align -Wcast-qual -Wdouble-promotion -Weffc++ -Wfloat-equal
    -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2
    -Wimplicit-fallthrough=5 -Winit-self -Winline -Wmain -Wmissing-braces
    -Wmissing-declarations -Wmissing-include-dirs -Wnull-dereference -Wpadded
    -Wredundant-decls -Wshadow=global -Wswitch-default -Wswitch-enum -Wundef
    -Wunreachable-code -Wunused-const-variable=2 -Wunused-macros
    -Wzero-as-null-pointer-constant
)
set(LDFLAGS -s)

add_executable(${TARGET} main.cpp
    index.cpp
    indexer.cpp
    memmap.cpp
    normalizer.cpp
    stemmer.cpp
    tokenizer.cpp
)
target_compile_options(${TARGET} PRIVATE
    "$<$<CXX_COMPILER_ID:GNU>:${CXXFLAGS}>"
)
target_include_directories(${TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_options(${TARGET} PRIVATE
    "$<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:RELEASE>:${LDFLAGS}>>"
)
