#pragma once

#include "BaseException.h"
#include <string>

namespace ErrorHandling::Exceptions
{
    class UserNotFoundException : public BaseException
    {
        public:
            explicit UserNotFoundException(const std::string& commandLine, const std::string& failureReason);
    };
}
