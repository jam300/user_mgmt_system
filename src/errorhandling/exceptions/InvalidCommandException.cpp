#include "errorhandling/exceptions/InvalidCommandException.h"

namespace ErrorHandling::Exceptions
{
    InvalidCommandException::InvalidCommandException(const std::string& commandName, const std::string& failureReason)
        : BaseException(commandName, "Invalid Command: " + failureReason)
    {
    }
}