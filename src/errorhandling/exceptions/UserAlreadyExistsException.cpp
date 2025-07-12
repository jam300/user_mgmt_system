#include "errorhandling/exceptions/UserAlreadyExistsException.h"

namespace ErrorHandling::Exceptions
{
    UserAlreadyExistsException::UserAlreadyExistsException(const std::string& commandLine, const std::string& failureReason)
        : BaseException(commandLine, failureReason)
    {
    }
}