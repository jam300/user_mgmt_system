#include "commands/ExitCommand.h"

namespace Commands
{
    void ExitCommand::execute(Domain::SystemState&)
    {
        m_triggered = true;
    }

    bool ExitCommand::wasTriggered()
    {
        return m_triggered;
    }

    void ExitCommand::reset()
    {
        m_triggered = false;
    }
}