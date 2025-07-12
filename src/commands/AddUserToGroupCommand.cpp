#include "commands/AddUserToGroupCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
namespace Commands
{
    AddUserToGroupCommand::AddUserToGroupCommand(std::string username_, std::string groupName_)
            : m_username(std::move(username_)), m_groupname(std::move(groupName_)) {}

    void AddUserToGroupCommand::execute(Domain::SystemState &state)
    {
        state.AddUserToGroup(std::move(m_username), std::move(m_groupname));
        OutputPrinter::PrintCommandSuccess("ADD USER " + m_username + " TO GROUP " + m_groupname);
    }

}
