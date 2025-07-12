#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class GetMessageHistoryCommand : public ICommand
    {
        public:
            explicit GetMessageHistoryCommand(std::string username_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_username;
    };
}