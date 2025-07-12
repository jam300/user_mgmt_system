#include "errorhandling/exceptions/CommandExecutionException.h"

namespace ErrorHandling::Exceptions
{
    CommandExecutionException::CommandExecutionException(const std::string& commandName, const std::string& failureReason)
        : BaseException(commandName, failureReason)
    {
    }
}