#include "commands/DisableUserCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
namespace Commands
{
    DisableUserCommand::DisableUserCommand(std::string username_)
                : m_username(std::move(username_)) {}

    void DisableUserCommand::execute(Domain::SystemState& state)
    {
        state.DisableUser(m_username);
        OutputPrinter::PrintCommandSuccess("DISABLE USER " + m_username);
    }

}