#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class SendMessageCommand : public ICommand
    {
        public:
            explicit SendMessageCommand(std::string toUsername_, std::string message_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_toUsername;
            std::string m_message;
    };
}