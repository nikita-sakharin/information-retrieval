set(BINARY ${PROJECT_NAME}_test)

add_executable(${BINARY}
    ${PROJECT_SOURCE_DIR}/src/tokenizator.cpp
    tokenizator.test.cpp
    abc.test.cpp
)
target_include_directories(${BINARY} PRIVATE
    "${PROJECT_SOURCE_DIR}/lib/googletest/googletest/include"
    "${PROJECT_SOURCE_DIR}/include"
)
target_link_libraries(${BINARY} gtest_main)
