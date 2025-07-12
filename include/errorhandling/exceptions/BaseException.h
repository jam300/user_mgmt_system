#pragma once

#include <exception>
#include <string>

namespace ErrorHandling::Exceptions
{
    class BaseException : public std::exception
    {
        public:
            explicit BaseException(std::string commandLine_, std::string failureReason_);

            const char* what() const noexcept override;
            const std::string& GetCommandLine() const;
            const std::string& GetFailureReason() const;

        protected:
            std::string m_commandLine;
            std::string m_failureReason;
            std::string m_fullMessage;
    };
}
