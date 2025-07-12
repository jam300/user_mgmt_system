#pragma once

#include <string>
#include <vector>
#include <memory>
#include "domain/SystemState.h"
#include "app/TaskFileLoader.h"
#include "app/TasksParser.h"
#include "commands/ICommand.h"
#include "utils/Types.h"

namespace App
{
    class TaskManager
    {
        public:
            explicit TaskManager(const std::string& taskDirectoryPath);

            void SetState(std::shared_ptr<Domain::SystemState> state);
            void UpdateTasksPath(const std::string& newPath);
            void RunTasksFromFiles();

        private:
            std::shared_ptr<Domain::SystemState> m_state;
            std::unique_ptr<App::TaskFileLoader> m_loader;
            std::unique_ptr<App::TasksParser> m_parser;
            App::CommandRegistry m_registry;
    };
}