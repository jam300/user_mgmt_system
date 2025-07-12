#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class ExitCommand : public ICommand
    {
        public:
            ExitCommand() = default;

            void execute(Domain::SystemState& state) override;

            static bool wasTriggered();
            static void reset();

        private:
            inline static bool m_triggered = false;
    };
}