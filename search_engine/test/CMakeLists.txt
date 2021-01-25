set(BINARY ${PROJECT_NAME}_test)

add_executable(${BINARY}
    ${PROJECT_SOURCE_DIR}/src/index.cpp
    ${PROJECT_SOURCE_DIR}/src/indexer.cpp
    ${PROJECT_SOURCE_DIR}/src/memmap.cpp
    char_encoder.test.cpp
    index.test.cpp
    memmap.test.cpp
    str_encoder.test.cpp
    str_parser.test.cpp
    tokenizer.test.cpp
)
target_include_directories(${BINARY} PRIVATE
    "${PROJECT_SOURCE_DIR}/lib/googletest/googletest/include"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${BINARY} gtest_main)
