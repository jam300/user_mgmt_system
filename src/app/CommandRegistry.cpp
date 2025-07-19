#include "app/CommandRegistry.h"
#include "commands/AddUserToGroupCommand.h"
#include "commands/CreateUserCommand.h"
#include "commands/DelateUserCommand.h"
#include "commands/DisableUserCommand.h"
#include "commands/ExitCommand.h"
#include "commands/GetGroupsCommand.h"
#include "commands/GetMessageHistoryCommand.h"
#include "commands/GetUsersCommand.h"
#include "commands/PingCommand.h"
#include "commands/RemoveUserFromGroupCommand.h"
#include "commands/SendMessageCommand.h"
#include "utils/CommandStrings.h"
#include "errorhandling/exceptions/AllExceptions.h"
#include <iostream>
#include<stdexcept>
using namespace CMD;
using namespace ErrorHandling::Exceptions;
namespace App
{
    /**
     * @brief Constructs and initializes the CommandRegistry with all available commands.
     *Registers all command keys with their corresponding command creation logic.
     *
     * @throws InvalidArgumentException if the number of arguments for a command is incorrect.
     */
    CommandRegistry::CommandRegistry()
    {
        registerCommand(CMD_ADD_USER_TO_GROUP, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 2) throw InvalidArgumentException(std::string(CMD_ADD_USER_TO_GROUP), " Command Expects 2 Argument.");
                        return std::make_unique<Commands::AddUserToGroupCommand>(args[0], args[1]);
                    });

        registerCommand(CMD_CREATE_USER, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 1)throw InvalidArgumentException(std::string(CMD_CREATE_USER),  " Command Expects 1 Argument.");
                        return std::make_unique<Commands::CreateUserCommand>(args[0]);
                    });

        registerCommand(CMD_DELETE_USER, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 1)throw InvalidArgumentException(std::string(CMD_DELETE_USER), " Command Expects 1 Argument.");
                        return std::make_unique<Commands::DeleteUserCommand>(args[0]);
                    });

        registerCommand(CMD_DISABLE_USER, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 1)throw InvalidArgumentException(std::string(CMD_DISABLE_USER), " Command Expects 1 Argument.");
                        return std::make_unique<Commands::DisableUserCommand>(args[0]);
                    });

        registerCommand(CMD_EXIT, [](const std::vector<std::string>& args)
                    {
                        if(!args.empty())throw InvalidArgumentException(std::string(CMD_EXIT)," Command Expects NO Arguments.");
                        return std::make_unique<Commands::ExitCommand>();
                    });

        registerCommand(CMD_GET_GROUPS, [](const std::vector<std::string>& args)
                    {
                        if(!args.empty())throw InvalidArgumentException(std::string(CMD_GET_GROUPS)," Command Expects NO Arguments.");
                        return std::make_unique<Commands::GetGroupsCommand>();
                    });

        registerCommand(CMD_GET_MESSAGE_HISTORY, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 1)throw InvalidArgumentException(std::string(CMD_GET_MESSAGE_HISTORY), " Command Expects 1 Argument.");
                        return std::make_unique<Commands::GetMessageHistoryCommand>(args[0]);
                    });

        registerCommand(CMD_GET_USERS, [](const std::vector<std::string>& args)
                    {
                        if(!args.empty())throw InvalidArgumentException(std::string(CMD_GET_USERS), " Command Expects NO Arguments.");
                        return std::make_unique<Commands::GetUsersCommand>();
                    });

        registerCommand(CMD_PING, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 2)throw InvalidArgumentException(std::string(CMD_PING), " Command Expects 2 Argument.");
                        return std::make_unique<Commands::PingCommand>(args[0], args[1]);
                    });

        registerCommand(CMD_REMOVE_USER_FROM_GROUP, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 2)throw InvalidArgumentException(std::string(CMD_REMOVE_USER_FROM_GROUP), " Command Expects 2 Arguments.");
                        return std::make_unique<Commands::RemoveUserFromGroupCommand>(args[0], args[1]);
                    });

        registerCommand(CMD_SEND_MESSAGE, [](const std::vector<std::string>& args)
                    {
                        if(args.size() != 2)throw InvalidArgumentException(std::string(CMD_SEND_MESSAGE)," Command Expects 2 Arguments.");
                        return std::make_unique<Commands::SendMessageCommand>(args[0], args[1]);
                    });
    }
    /**
     * @brief Registers a new command into the registry.
     *
     * @param commandKey The key that identifies the command (usually an uppercase string like "ADD USER").
     * @param command_task A factory lambda or function that returns a new command object based on input arguments.
     */
    void CommandRegistry::registerCommand(const std::string &commandKey, CommandFactory command_task)
    {
        m_registryMap[commandKey] = std::move(command_task);
    }
    /**
     * @brief Creates a command instance from the registered commands.
     *
     * @param commandName The name/key of the command to create.
     * @param args The arguments to be passed to the command constructor.
     * @return std::unique_ptr<ICommand> The constructed command object.
     *
     * @throws InvalidCommandException if the command name is not registered.
     * @throws InvalidArgumentException if the argument count is invalid for the given command.
     */
    std::unique_ptr<Commands::ICommand> CommandRegistry::createCommand(const std::string &commnadName, const std::vector<std::string> &args) const
    {
        auto itr = m_registryMap.find(commnadName);

        if(itr == m_registryMap.end())
            throw InvalidCommandException(commnadName, "Does not exist");

        return itr->second(args);
    }

}