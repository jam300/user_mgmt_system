#include "domain/Message.h"

namespace Domain
{
    Message::Message( std::string content_): m_content(std::move(content_)){}

    const std::string& Message::getContent() const
    {
        return m_content;
    }
}