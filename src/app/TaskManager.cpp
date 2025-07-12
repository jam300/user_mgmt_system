#include "app/TaskManager.h"
#include "commandresult/OutputPrinter.h"
#include "errorhandling/ErrorHandler.h"
#include "commands/ExitCommand.h"

using namespace TasksTypes;
using CommandResult::OutputPrinter;
using namespace ErrorHandling::Exceptions;

namespace App
{
    TaskManager::TaskManager(const std::string& taskDirectoryPath)
    {
        m_loader = std::make_unique<TaskFileLoader>(std::move(taskDirectoryPath));
        m_parser = std::make_unique<TasksParser>(m_registry);
    }

    void TaskManager::SetState(std::shared_ptr<Domain::SystemState> state)
    {
        m_state = std::move(state);
    }

    void TaskManager::UpdateTasksPath(const std::string& newPath)
    {
        m_loader = std::make_unique<TaskFileLoader>(newPath);
    }

    void TaskManager::RunTasksFromFiles()
    {
        if (!m_state)
        {
            //throw std::runtime_error("[TaskManager] SystemState has not been set!");
        }
        try
        {
            ListOfTaskFiles taskFiles = m_loader->LoadAllTasks();
            for (const auto& [fileName, lines] : taskFiles)
            {
                auto name = fileName;
                OutputPrinter::PrintTaskStart(fileName);
                ParsedTasks parsed = m_parser->ParseTasks(fileName, lines);

                if (parsed.empty())
                {
                    OutputPrinter::PrintTaskFailure(fileName);
                    continue;
                }
                bool executionFailedForThisFile = false;
                for (const auto& [commandName, commands] : parsed)
                {
                    for (auto& command : commands)
                    {
                        if (command)
                        {
                            try
                            {
                                command->execute(*m_state);
                            }
                            catch(const BaseException& e)
                            {
                                ErrorHandler::Handle(e, "Command Execution");
                                executionFailedForThisFile = true;
                                break;
                            }
                        }
                        if (Commands::ExitCommand::wasTriggered())
                        {
                            Commands::ExitCommand::reset();
                            break;
                        }
                    }
                    if (executionFailedForThisFile || Commands::ExitCommand::wasTriggered())
                    {
                        break;
                    }
                }
                if (executionFailedForThisFile)
                {
                    OutputPrinter::PrintTaskFailure(fileName);
                }
                else if (!Commands::ExitCommand::wasTriggered())
                {
                    OutputPrinter::PrintTaskSuccess(fileName);
                }
            }
        }
        catch(const BaseException& e)
        {
            ErrorHandler::Handle(e, "TaskManager->RunTasksFromFiles");
        }
    }
}