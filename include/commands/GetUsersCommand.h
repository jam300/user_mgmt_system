#pragma once

#include <string>
#include "commands/ICommand.h"
#include "domain/SystemState.h"

namespace Commands
{
    class GetUsersCommand : public ICommand
    {
        public:
            GetUsersCommand() = default;

            void execute(Domain::SystemState& state) override;
    };
}