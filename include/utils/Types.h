#pragma once
#include "commands/ICommand.h"
#include <functional>
#include<memory>
#include <vector>

namespace TasksTypes
{
    using CommandList = std::vector<std::unique_ptr<Commands::ICommand>>;
    using ParsedTasks = std::vector<std::pair<std::string, CommandList>>;
    using CommandFactory = std::function<std::unique_ptr<Commands::ICommand>(const std::vector<std::string>&)>;
    using TaskFile = std::pair<std::string, std::vector<std::string>>;
    using ListOfTaskFiles = std::vector<TaskFile>;
}