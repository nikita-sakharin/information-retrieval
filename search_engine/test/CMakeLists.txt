include_directories(include)

add_executable(test tokenizator.test.cpp abc.test.cpp)

target_include_directories(test PRIVATE
    "${PROJECT_SOURCE_DIR}/lib/googletest/googletest/include"
)
target_link_libraries(test gtest gtest_main)
