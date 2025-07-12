#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class CreateUserCommand : public ICommand
    {
        public:
            explicit CreateUserCommand(std::string username_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_username;
    };
}