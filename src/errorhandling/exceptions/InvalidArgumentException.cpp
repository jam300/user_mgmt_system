#include "errorhandling/exceptions/InvalidArgumentException.h"

namespace ErrorHandling::Exceptions
{
    InvalidArgumentException::InvalidArgumentException(const std::string& commandName, const std::string& failureReason)
        : BaseException(commandName, failureReason)
    {
    }
}