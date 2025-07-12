#pragma once

#include <string>
#include <iostream>

namespace CommandResult
{
    class OutputPrinter
    {
        public:
            static void PrintCommandSuccess(const std::string& commandLine);
            static void PrintCommandResult(const std::string& commandLine);
            static void PrintCommandFailure(const std::string& commandLine, const std::string& failure);
            static void PrintTaskStart(const std::string& taskName);
            static void PrintTaskFailure(const std::string& taskName);
            static void PrintTaskSuccess(const std::string& taskName);
    };
}