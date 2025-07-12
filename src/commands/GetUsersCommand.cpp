#include "commands/GetUsersCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
namespace Commands
{
    void GetUsersCommand::execute(Domain::SystemState& state)
    {
        const auto& users = state.getUsers();
        OutputPrinter::PrintCommandSuccess("GET USERS" );
        std::for_each(users.begin(), users.end(), [](const std::shared_ptr<Domain::User>& user) {
                    OutputPrinter::PrintCommandResult(user->getUsername());
                });
    }


}