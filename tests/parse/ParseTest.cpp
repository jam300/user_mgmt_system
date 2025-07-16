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

TEST(ParserTests, ArgumentParser_Word)
{
    auto parser = argument_parser();

    std::string input = "Javi say: \"hello world\"";
    auto result = parser(input, 0);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.value(), "Javi");

    std::string input2 = "\"hello world\" from Adan";
    auto result2 = parser(input2, 0);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result2.value(),  "hello world");
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

/*TEST(ParserTests, WhenNumberInStringThenfunctionStop)
{
    auto parser = uppercase_word_parser();
    auto result = parser("ADMIN1", 0);

    EXPECT_TRUE(result.success());
    EXPECT_EQ(result.value(), "ADMIN");
    EXPECT_EQ(result.index(), 5);
}*/

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

TEST(ParserTests, WhesMultipleUppercaseWordsSeparatedBySpacesThenSuccess)
{
    auto parser = command_name_parser();
    auto result = parser("ADD USER TO GROUP", 0);

    EXPECT_TRUE(result.success());
    EXPECT_EQ(result.value(), "ADD USER TO GROUP");
    EXPECT_EQ(result.index(), 17);
}

TEST(ParserTests, WhenExtraSpacesBetweenWordsFunctionIgnoreAndSuccess)
 {
    auto parser = command_name_parser();
    auto result = parser("ADD   USER   TO   GROUP", 0);

    EXPECT_TRUE(result.success());
    EXPECT_EQ(result.value(), "ADD USER TO GROUP");
    EXPECT_EQ(result.index(), 23);
}

TEST(ParserTests, FailsOnLowercaseWord) {
    auto parser = command_name_parser();
    auto result = parser("Add User", 0);
    EXPECT_FALSE(result.success());
}

TEST(CommandNameParserTest, WhenLineWitnUpperandNotUpperCaseThenFunctionJustTakeUpperCase)
{
    auto parser = command_name_parser();
    auto result = parser("ADD USER alice TO GROUP admin", 0);

    EXPECT_TRUE(result.success());
    EXPECT_EQ(result.value(), "ADD USER TO GROUP");
}

TEST(CommandNameParserTest, WhenInputStringEmptyThenfails)
{
    auto parser = command_name_parser();
    auto result = parser("", 0);

    EXPECT_FALSE(result.success());
}
