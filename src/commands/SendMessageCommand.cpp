#include "commands/SendMessageCommand.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;

namespace Commands
{
    SendMessageCommand::SendMessageCommand(std::string toUsername_, std::string message_)
            : m_toUsername(std::move(toUsername_)), m_message(std::move(message_)) {}

    void SendMessageCommand::execute(Domain::SystemState &state)
    {
        auto message = std::make_unique<Domain::Message>(m_message);
        state.SendMessage(m_toUsername, std::move(message));

        OutputPrinter::PrintCommandSuccess("SEND MASSAGE " + m_toUsername + " " + m_message);
    }
}
