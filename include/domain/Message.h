#pragma once

#include <string>

namespace Domain
{
    class Message
    {
        public:
            explicit Message( std::string content_);

            const std::string& getContent() const;

        private:
            std::string m_content;
    };
}