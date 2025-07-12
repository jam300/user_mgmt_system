#include "commands/GetGroupsCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
namespace Commands
{
    void GetGroupsCommand::execute(Domain::SystemState& state)
    {
        const auto& groups = state.getGroups();
        OutputPrinter::PrintCommandSuccess("GET GROUPS" );
        std::for_each(groups.begin(), groups.end(), [](const std::shared_ptr<Domain::Group>& group) {
                    OutputPrinter::PrintCommandResult(group->getGroupName());
                });
    }
}