#include "commands/CreateUserCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;

namespace Commands
{
    CreateUserCommand::CreateUserCommand(std::string username_)
                : m_username(std::move(username_)) {}

    void CreateUserCommand::execute(Domain::SystemState& state)
    {
        auto newUser = std::make_shared<Domain::User>(m_username);
        state.AddUser(newUser);
        OutputPrinter::PrintCommandSuccess("CREATE USER " + m_username);
    }
}

