#include <cassert>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include <search_engine/header.hpp>

TEST(AbcTest, Abc) {
    std::cout << "AbcTest, Abc" << std::endl;
    ASSERT_EQ(2 + 2, 4);
    ASSERT_EQ(std::string("a") + "b" + "c", "abc");
}
