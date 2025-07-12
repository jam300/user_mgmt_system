#include "commands/DelateUserCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;

namespace Commands
{
    DeleteUserCommand::DeleteUserCommand(std::string username_)
                : m_username(std::move(username_)) {}

    void DeleteUserCommand::execute(Domain::SystemState& state)
    {
        state.DeleteUser(m_username);
        OutputPrinter::PrintCommandSuccess("DELETE USER " + m_username);
    }

}