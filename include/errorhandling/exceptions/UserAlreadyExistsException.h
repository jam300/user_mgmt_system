#pragma once

#include "BaseException.h"
#include <string>

namespace ErrorHandling::Exceptions
{
    class UserAlreadyExistsException : public BaseException
    {
        public:
            explicit UserAlreadyExistsException(const std::string& commandLine, const std::string& failureReason);
    };
}
