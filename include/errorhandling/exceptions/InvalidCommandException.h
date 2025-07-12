#pragma once

#include "BaseException.h"
#include <string>

namespace ErrorHandling::Exceptions
{
    class InvalidCommandException : public BaseException
    {
        public:
            explicit InvalidCommandException(const std::string& commandName, const std::string& failureReason);
    };
}
