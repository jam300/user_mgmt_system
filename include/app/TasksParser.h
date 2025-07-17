#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "parsec/parsec.hpp"
#include "commands/ICommand.h"
#include "CommandRegistry.h"
#include "utils/Types.h"

namespace App
{
    class TasksParser
    {
        public:

            explicit TasksParser(const CommandRegistry& registry);

            TasksTypes::ParsedTasks ParseTasks(const std::string& fileName, const std::vector<std::string>& rawTasks) const;

        private:
            const CommandRegistry& m_registry;
            std::string CleanLine(const std::string &line) const;
            TasksTypes::TaskFile ExtractCommandAndArgs(const std::string &line) const;
    };


    parsec::Parser<char> char_p(char c);
    parsec::Parser<char> char_p_if(std::function<bool(char)> condition, std::string msg);
    parsec::Parser<std::string> spaces();
    parsec::Parser<std::string> spaces1();
    parsec::Parser<std::string> word_parser();
    parsec::Parser<std::string> quoted_string_parser();
    parsec::Parser<TasksTypes::TaskFile> combined_command_and_argument_parser();
    parsec::Parser<std::string> uppercase_word_parser();
    parsec::Parser<std::string> command_name_parser();
}