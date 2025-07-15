#include <gtest/gtest.h>
#include "parsec/parsec.hpp"
#include "app/TaskManager.h"
using namespace App;
using namespace parsec;

TEST(ParserBasicsTest, CharPIfWorks)
{
    auto is_upper = [](char c) { return std::isupper(static_cast<unsigned char>(c)); };
    auto parser = char_p_if(is_upper, "is upper");

    auto res = parser("Hello", 0);
    ASSERT_TRUE(res.success());
    EXPECT_EQ(res.value(), 'H');
    EXPECT_EQ(res.index(), 1);

    auto resFail = parser("hello", 0);
    ASSERT_FALSE(resFail.success());
}

TEST(ParserBasicsTest, CharPWorks)
{
    auto parser = char_p('A');

    auto res = parser("ABC", 0);
    ASSERT_TRUE(res.success());
    EXPECT_EQ(res.value(), 'A');
    EXPECT_EQ(res.index(), 1);

    auto resFail = parser("XYZ", 0);
    ASSERT_FALSE(resFail.success());
    EXPECT_EQ(resFail.index(), 0);
}

TEST(ParserBasicsTest, SpacesWorks)
{
    auto parser = spaces();
    auto res = parser("   test", 0);
    ASSERT_TRUE(res.success());
    EXPECT_EQ(res.value(), "   ");
    EXPECT_EQ(res.index(), 3);

    auto resNone = parser("test", 0);
    ASSERT_TRUE(resNone.success());
    EXPECT_EQ(resNone.value(), "");
    EXPECT_EQ(resNone.index(), 0);
}

TEST(ParserBasicsTest, Spaces1Works)
{
    auto parser = spaces1();
    auto res = parser("   test", 0);
    ASSERT_TRUE(res.success());
    EXPECT_EQ(res.value(), "   ");
    EXPECT_EQ(res.index(), 3);

    auto resFail = parser("test", 0);
    ASSERT_FALSE(resFail.success());
    EXPECT_EQ(resFail.index(), 0);
}
