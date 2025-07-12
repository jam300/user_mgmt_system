#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>

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
}