#pragma once

#include "domain/SystemState.h"

namespace Commands
{
    class ICommand
    {
        public:
            virtual void execute(Domain::SystemState& state) = 0;
            virtual ~ICommand() = default;
    };
}