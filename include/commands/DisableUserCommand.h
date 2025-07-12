#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class DisableUserCommand : public ICommand
    {
        public:
            explicit DisableUserCommand(std::string username_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_username;
    };
}