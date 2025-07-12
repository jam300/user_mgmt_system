#include "commands/GetMessageHistoryCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
namespace Commands
{
    GetMessageHistoryCommand::GetMessageHistoryCommand(std::string username_)
                : m_username(std::move(username_)) {}

    void GetMessageHistoryCommand::execute(Domain::SystemState& state)
    {
        const auto& messageHistory = state.getMessageHistory(m_username);
        OutputPrinter::PrintCommandSuccess("GET MESSAGE HISTORY " + m_username);
        std::for_each(messageHistory.begin(), messageHistory.end(), [](const std::unique_ptr<Domain::Message>& msg) {
                    OutputPrinter::PrintCommandResult(msg->getContent());
                });
    }
}