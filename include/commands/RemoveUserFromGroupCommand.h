#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class RemoveUserFromGroupCommand : public ICommand
    {
        public:
            explicit RemoveUserFromGroupCommand(std::string username_, std::string groupName_);

            void execute(Domain::SystemState& state) override;

        private:
            std::string m_username;
            std::string m_groupname;
    };
}