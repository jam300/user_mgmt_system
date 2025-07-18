#include "domain/Message.h"

namespace Domain
{
    /**
     * @brief Constructs a Message with the given content.
     * @param content_ The text content of the message.
     */
    Message::Message( std::string content_): m_content(std::move(content_)){}
    /**
     * @brief Retrieves the content of the message.
     * @return const std::string& A reference to the string containing the message content.
     */
    const std::string& Message::getContent() const
    {
        return m_content;
    }
}