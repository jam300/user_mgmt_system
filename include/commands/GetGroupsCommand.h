#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class GetGroupsCommand : public ICommand
    {
        public:
            GetGroupsCommand() = default;

            void execute(Domain::SystemState& state) override;
    };
}