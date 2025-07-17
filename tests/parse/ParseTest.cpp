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

TEST(ParserTests, WhenCompletewordThenWordParserSuccess)
{
    auto parser = word_parser();

    std::string input = "hello_world123";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.value(), "hello_world123");
    EXPECT_EQ(result.index(), input.size());
}

TEST(ParserTests, WhenStringHasSpaceThenWordParserStopsAtSpace)
{
    auto parser = word_parser();

    std::string input = "hello world";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.value(), "hello");
    EXPECT_EQ(result.index(), 5);
}

TEST(ParserTests, WhenInputIsEmptyThenWordParserFails)
{
    auto parser = word_parser();

    std::string input = "";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.failure());
    EXPECT_EQ(result.index(), 0);
}

TEST(ParserTests, WhenCorrectQuotedStringParserThenSuccess)
{
    auto parser = quoted_string_parser();

    std::string input = "\"hello world\"";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.value(), "hello world");
    EXPECT_EQ(result.index(), input.size());
}

TEST(ParserTests, WhenEmptyStringThenQuotedStringParserRetunEmpty)
{
    auto parser = quoted_string_parser();

    std::string input = "\"\"";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.value(), "");
    EXPECT_EQ(result.index(), input.size());
}

TEST(ParserTests, WhenClosingQuotedIsMissingThenQuotedStringParserFails)
{
    auto parser = quoted_string_parser();

    std::string input = "\"hello world";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.failure());
    EXPECT_EQ(result.index(), input.size());
}

TEST(ParserTests, WhenOpenQuoteIsMissingThenQuotedStringParserFails)
{
    auto parser = quoted_string_parser();

    std::string input = "hello world\"";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.failure());
    EXPECT_EQ(result.index(), 0);
}

TEST(ParserTests, whenUppercaseWordThenSuccess)
{
    auto parser = uppercase_word_parser();
    auto result = parser("HELLO", 0);

    EXPECT_TRUE(result.success());
    EXPECT_EQ(result.value(), "HELLO");
    EXPECT_EQ(result.index(), 5);
}

TEST(ParserTests, WhenLowerCaseWordThenFails)
{
    auto parser = uppercase_word_parser();
    auto result = parser("hello", 0);

    EXPECT_FALSE(result.success());
}

TEST(ParserTests, WhenLowerandUpperCaseWordMixThenFails)
{
    auto parser = uppercase_word_parser();
    auto result = parser("Hello", 0);
    EXPECT_FALSE(result.success());

    auto result2 = parser("heLlo", 0);
    EXPECT_FALSE(result2.success());
}

TEST(ParserTests, WhenEmptyStringTheFunctionFails)
{
    auto parser = uppercase_word_parser();
    auto result = parser("", 0);

    EXPECT_FALSE(result.success());
}

TEST(ParserTests, WhenSpaceInStringTheFunctionFails)
{
    auto parser = uppercase_word_parser();
    auto result = parser(" ADMIN", 0);

    EXPECT_FALSE(result.success());
}

TEST(CommandNameParserTest, WhenLineWitnUpperandNotUpperCaseThenFunctionJustTakeUpperCase)
{
    auto parser = combined_command_and_argument_parser();
    auto result = parser("ADD USER alice TO GROUP admin", 0);

    ASSERT_TRUE(result.success());

    const auto& [command, arguments] = result.value();

    EXPECT_EQ(command, "ADD USER TO GROUP");

    ASSERT_EQ(arguments.size(), 2);
    EXPECT_EQ(arguments.front(), "alice");
    EXPECT_EQ(arguments.back(), "admin");
}

TEST(CommandNameParserTest, test1)
{
    auto parser = combined_command_and_argument_parser();
    auto result = parser("ADD USER Javi", 0);

    ASSERT_TRUE(result.success());

    const auto& [command, arguments] = result.value();

    EXPECT_EQ(command, "ADD USER");

    ASSERT_EQ(arguments.size(), 1);
    EXPECT_EQ(arguments.front(), "Javi");
}

TEST(CommandNameParserTest, test2)
{
    auto parser = combined_command_and_argument_parser();
    auto result = parser("EXIT", 0);

    ASSERT_TRUE(result.success());

    const auto& [command, arguments] = result.value();

    EXPECT_EQ(command, "EXIT");

    ASSERT_EQ(arguments.size(), 0);
}

TEST(CommandNameParserTest, test3)
{
    auto parser = combined_command_and_argument_parser();
    auto result = parser("SEND MESSAGE Javi  \"hello world\"", 0);

    ASSERT_TRUE(result.success());

    const auto& [command, arguments] = result.value();

    EXPECT_EQ(command, "SEND MESSAGE");

    ASSERT_EQ(arguments.size(), 2);
    EXPECT_EQ(arguments.front(), "Javi");
    EXPECT_EQ(arguments.back(), "hello world");
}
