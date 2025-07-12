#include "errorhandling/exceptions/BaseException.h"

namespace ErrorHandling::Exceptions
{
    BaseException::BaseException(std::string commandLine_, std::string failureReason_)
    : m_commandLine(std::move(commandLine_)),
    m_failureReason(std::move(failureReason_))
    {
        m_fullMessage =  m_commandLine + m_failureReason;
    }

    const char* BaseException::what() const noexcept
    {
        return m_fullMessage.c_str();
    }

    const std::string& BaseException::GetCommandLine() const
    {
        return m_commandLine;
    }

    const std::string& BaseException::GetFailureReason() const
    {
        return m_failureReason;
    }
}
