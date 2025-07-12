#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>

#include "commands/ICommand.h"

namespace App
{
    using CommandFactory = std::function<std::unique_ptr<Commands::ICommand>(const std::vector<std::string>&)>;

    class CommandRegistry
    {
        public:

            CommandRegistry();
            ~CommandRegistry() = default;

            void registerCommand(const std::string& commandKey, CommandFactory command_task);
            std::unique_ptr<Commands::ICommand> createCommand(const std::string& commandName, const std::vector<std::string>& args) const;
            std::vector<std::string> GetAllCommandRegistry() const;
        private:
            std::unordered_map<std::string, CommandFactory> m_registryMap;
    };
}
