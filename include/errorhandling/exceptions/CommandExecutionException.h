#pragma once

#include "BaseException.h"
#include <string>

namespace ErrorHandling::Exceptions
{
    class CommandExecutionException : public BaseException
    {
        public:
            explicit CommandExecutionException(const std::string& commandName, const std::string& failureReason);
    };
}
