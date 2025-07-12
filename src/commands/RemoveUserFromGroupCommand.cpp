#include "commands/RemoveUserFromGroupCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
namespace Commands
{
    RemoveUserFromGroupCommand::RemoveUserFromGroupCommand(std::string username_, std::string groupName_)
            : m_username(std::move(username_)), m_groupname(std::move(groupName_)) {}

    void RemoveUserFromGroupCommand::execute(Domain::SystemState &state)
    {
        state.RemoveUserFromGroup(m_username, m_groupname);
        OutputPrinter::PrintCommandSuccess("ROMOVE USER " + m_username + " FROM GROUP " + m_groupname);
    }
}
