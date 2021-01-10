#include <cassert>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <search_engine/header.hpp>

TEST(TokenizerTest, WcharToChar) {
    std::cout << "TokenizerTest, WcharToChar" << std::endl;
    ASSERT_EQ('q', L'q');
}
