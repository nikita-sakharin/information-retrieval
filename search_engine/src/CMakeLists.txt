include_directories(include)

set(CMAKE_CXX_EXTENSIONS OFF)

set(BINARY ${PROJECT_NAME})

add_executable(${BINARY} main.cpp tokenizator.cpp)
target_include_directories(${BINARY} PRIVATE "${PROJECT_SOURCE_DIR}/include")
