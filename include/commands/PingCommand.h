#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class PingCommand : public ICommand
    {
        public:
            explicit PingCommand(std::string toUsername_, std::string times_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_toUsername;
            int m_times = 1;
    };
}