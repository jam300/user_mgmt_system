#include "errorhandling/exceptions/UserNotFoundException.h"

namespace ErrorHandling::Exceptions
{
    UserNotFoundException::UserNotFoundException(const std::string& commandLine, const std::string& failureReason)
        : BaseException(commandLine, failureReason)
    {
    }
}