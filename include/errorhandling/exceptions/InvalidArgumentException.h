#pragma once

#include "BaseException.h"
#include <string>

namespace ErrorHandling::Exceptions
{
    class InvalidArgumentException : public BaseException
    {
        public:
            explicit InvalidArgumentException(const std::string& commandName, const std::string& failureReason);
    };
}
