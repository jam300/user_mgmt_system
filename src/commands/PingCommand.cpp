#include "commands/PingCommand.h"
#include <stdexcept>
#include "errorhandling/exceptions/AllExceptions.h"
#include "commandresult/OutputPrinter.h"

using CommandResult::OutputPrinter;
using namespace  ErrorHandling::Exceptions;
namespace Commands
{

    PingCommand::PingCommand(std::string toUsername, std::string times_)
        : m_toUsername(std::move(toUsername))
    {
        try
        {
            m_times = std::stoi(times_);
        }
        catch (const std::exception&)
        {
            throw InvalidArgumentException("PING command ", " Invalid argument: "  + times_);
        }
    }

    void PingCommand::execute(Domain::SystemState& state)
    {
        bool isUser = state.isUserExists(m_toUsername);
        OutputPrinter::PrintCommandSuccess("Send Ping to " + m_toUsername + " (" + std::to_string(m_times) + ")");
        for(int i = 0; i < m_times; ++i)
        {
            OutputPrinter::PrintCommandResult("Sent Ping to " + m_toUsername);
            if(isUser)
            OutputPrinter::PrintCommandResult(m_toUsername + " received a ping");
        }

    }

}
