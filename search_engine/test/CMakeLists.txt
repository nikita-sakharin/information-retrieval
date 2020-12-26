set(BINARY ${PROJECT_NAME}_test)

add_executable(${BINARY}
    ${PROJECT_SOURCE_DIR}/src/index.cpp
    ${PROJECT_SOURCE_DIR}/src/memmap.cpp
    ${PROJECT_SOURCE_DIR}/src/tokenizer.cpp
    index.test.cpp
    memmap.test.cpp
    tokenizer.test.cpp
)
target_include_directories(${BINARY} PRIVATE
    "${PROJECT_SOURCE_DIR}/lib/googletest/googletest/include"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${BINARY} gtest_main)
