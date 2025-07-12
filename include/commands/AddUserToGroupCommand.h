#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class AddUserToGroupCommand : public ICommand
    {
        public:
            explicit AddUserToGroupCommand(std::string username_, std::string groupName_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_username;
            std::string m_groupname;
    };
}