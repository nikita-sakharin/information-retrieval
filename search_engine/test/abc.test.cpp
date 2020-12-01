#include <cassert>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <search_engine/header.hpp>

TEST(TokenizatorTest, Abc) {
    ASSERT_EQ(2 + 2, 4);
    ASSERT_EQ("a" + "b" + "c", "abc");
}
