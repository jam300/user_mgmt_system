#include "App/TasksParser.h"
#include "errorhandling/ErrorHandler.h"
#include <sstream>
#include <stdexcept>
#include <iostream>


using namespace ErrorHandling::Exceptions;
using namespace TasksTypes;

namespace App
{
    parsec::Parser<char> char_p(char c)
    {
        return [c](std::string_view input, size_t index) -> parsec::Result<char>{
            if(index < input.length() && input[index] == c)
            {
                return parsec::makeSuccess(static_cast<char>(c), index + 1, fmt::format("Matched '{}'", c), std::nullopt);
            }
            return parsec::makeError<char>(fmt::format("Expected '{}'", c), index);
        };
    }

    parsec::Parser<char> char_p_if(std::function<bool(char)> condition, std::string msg = "condition match")
    {
        return [condition = std::move(condition), msg = std::move(msg)](std::string_view input, size_t index) -> parsec::Result<char> {
            if (index < input.length() && condition(input[index])) {
                return parsec::makeSuccess(static_cast<char>(input[index]), index + 1, msg, std::nullopt);
            }
            return parsec::makeError<char>("Character did not match predicate", index);
        };
    }

    parsec::Parser<std::string> spaces()
    {
        return [](std::string_view input, size_t index) -> parsec::Result<std::string>{
            size_t start = index;
            while(index < input.length() && std::isspace(static_cast<unsigned char>(input[index]))) {index++;}

            return parsec::makeSuccess(std::string(input.substr(start, index - start)), index, "Whitespace", std::nullopt);
        };
    }

    parsec::Parser<std::string> spaces1()
    {
        return [](std::string_view input, size_t index) -> parsec::Result<std::string>{
            size_t start = index;
            while(index < input.length() && std::isspace(static_cast<unsigned char>(input[index]))) {index++;}

            if(index > start)
                return parsec::makeSuccess(std::string(input.substr(start, index - start)), index, "Whitespace", std::nullopt);

            return parsec::makeError<std::string>("Expected one or more whitespaces", start);
        };
    }

    parsec::Parser<std::string> word_parser()
    {
        auto word_char_p = char_p_if([](char c)
                {
                    return !std::isspace(static_cast<unsigned char>(c)) && c != '"';
                }, "Word character");

            return parsec::fmap<std::string, std::list<char>>(
                    [](std::list<char> chars)
                    {
                        return std::string(chars.begin(), chars.end());
                    },
                    many1(word_char_p)
        );
    }

    parsec::Parser<std::string> quoted_string_parser()
    {
        auto inner_content_parser = char_p('"') >> many(char_p_if([](char c){ return c != '"'; }, "Char inside quotes")) << char_p('"');
        return parsec::fmap<std::string, std::list<char>>(
                    [](std::list<char> chars) {
                        return std::string(chars.begin(), chars.end());
                    },
                    inner_content_parser
                );
    }

    parsec::Parser<std::string> uppercase_word_parser()
    {
        return [=](std::string_view sv, size_t i) -> parsec::Result<std::string>
        {

            auto wordResult = word_parser()(sv, i);

            if (wordResult.failure())
                return parsec::makeError<std::string>("Failed to parse word", i);

            std::string word = wordResult.value();

            if (std::all_of(word.begin(), word.end(), [](unsigned char c)
                {
                    return std::isupper(c);
                }))
            {
                return parsec::makeSuccess<std::string>(std::move(word), wordResult.index(), "Fully Uppercase",
                                std::make_optional<std::vector<parsec::Trace>>({std::move(wordResult.trace())}));
            }
            else
            {
                return parsec::makeError<std::string>("Word is not fully uppercase", i);
            }
        };
    }

    parsec::Parser<TasksTypes::TaskFile> combined_command_and_argument_parser()
    {
        auto token_classifier_parser = parsec::Parser<std::pair<std::optional<std::string>, std::optional<std::string>>>(
                [](std::string_view sv, size_t i) -> parsec::Result<std::pair<std::optional<std::string>, std::optional<std::string>>>
                {
                    auto uppercase_res = uppercase_word_parser()(sv, i);
                    if(uppercase_res.success())
                    {
                        return parsec::makeSuccess<std::pair<std::optional<std::string>, std::optional<std::string>>>(
                                { std::make_optional(std::move(uppercase_res.value())), std::nullopt },
                                    uppercase_res.index()
                            );
                    }

                    auto quoted_res = quoted_string_parser()(sv, i);
                    if (quoted_res.success())
                    {
                        return parsec::makeSuccess<std::pair<std::optional<std::string>, std::optional<std::string>>>(
                            { std::nullopt, std::make_optional(std::move(quoted_res.value())) },
                            quoted_res.index()
                        );
                    }

                    auto word_res = word_parser()(sv, i);
                    if (word_res.success())
                    {
                        return parsec::makeSuccess<std::pair<std::optional<std::string>, std::optional<std::string>>>(
                            { std::nullopt, std::make_optional(std::move(word_res.value())) },
                            word_res.index()
                        );
                    }

                    return parsec::makeError<std::pair<std::optional<std::string>, std::optional<std::string>>>(
                        "Token parsing failed", i);
                }
        );

        auto full_parser =  uppercase_word_parser() & parsec::many(spaces1() >> token_classifier_parser);

        return parsec::fmap<TaskFile, std::tuple<std::string, std::list<std::pair<std::optional<std::string>, std::optional<std::string>>>>>(
            [](const auto& parsed) -> TaskFile
            {
                std::string command_name = std::get<0>(parsed);
                std::vector<std::string> arguments;

                for (const auto& [cmd, arg] : std::get<1>(parsed))
                {
                    if (cmd.has_value())
                    {
                        command_name += " " + *cmd;
                    }
                    if (arg.has_value())
                    {
                        arguments.push_back(*arg);
                    }
                }

                return {std::move(command_name), std::move(arguments)};

            },full_parser);
    }

    TasksParser::TasksParser(const CommandRegistry &registry)
                : m_registry(registry){}

    ParsedTasks TasksParser::ParseTasks(const std::string& fileName, const std::vector<std::string>& rawTasks) const
    {
        ParsedTasks parsed;
        CommandList commands;
        bool taskHasError = false;

        for (const auto& rawline : rawTasks)
        {
            std::string newLine = CleanLine(rawline);

            if (newLine.empty())
                continue;

            try
            {
               // auto [commandName, args] = ExtractCommandAndArgs(newLine);
                auto parser = combined_command_and_argument_parser();
                auto result = parser(newLine, 0);
                const auto& [commandName, args] = result.value();

                auto command = m_registry.createCommand(commandName, args);
                commands.push_back(std::move(command));
            }
            catch (const BaseException& e)
            {
                ErrorHandler::Handle(e, "CreateCommandFromLine");
                taskHasError = true;
                break;
            }
        }

        if (!taskHasError)
            parsed.emplace_back(fileName, std::move(commands));

        return parsed;
    }

    std::string TasksParser::CleanLine(const std::string& line) const
    {

        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));

        if (trimmed.empty() || trimmed[0] == '#')
            return "";

        size_t commentPos = trimmed.find('#');
        if (commentPos != std::string::npos)
            trimmed = trimmed.substr(0, commentPos);


        trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
        return trimmed;
    }

    TaskFile TasksParser::ExtractCommandAndArgs(const std::string &line) const
    {
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string token;
        std::string quoted;

        while (iss >> std::ws)
        {
            if (iss.peek() == '"')
            {
                std::string quoted;
                iss.get();
                std::getline(iss, quoted, '"');
                tokens.push_back(quoted);

                if (iss.peek() == ' ')
                    iss.get();
            }
            else
            {
                iss >> token;
                tokens.push_back(token);
            }
        }

        std::string commandName;
        for(const std::string& word: tokens)
        {
            if( std::all_of(word.begin(), word.end(), [](unsigned char c){
                            return  std::isupper(c);
                        }))
            {
                if (!commandName.empty())
                    commandName += " ";
                commandName += word;
            }
        }

        std::vector<std::string> args;
        std::copy_if(tokens.begin(), tokens.end(), std::back_inserter(args), [&](const std::string& word) {
            std::string pattern = " " + word + " ";
            std::string paddedCommand = " " + commandName + " ";
            return paddedCommand.find(pattern) == std::string::npos;
        });

        return  {commandName, args};
    }
}

