#include "domain/User.h"
#include "domain/Group.h"
#include "domain/Message.h"
#include <algorithm>

namespace Domain
{
    User::User(std::string username_):m_userName(std::move(username_)){}

    const std::string& User::getUsername() const
    {
        return m_userName;
    }

    bool User::isDisabled() const
    {
        return m_disable;
    }

    void User::disable()
    {
        m_disable = true;
    }

    const std::vector<std::weak_ptr<Group>>& User::getGroups()const
    {
        return m_groups;
    }

    const std::vector<std::unique_ptr<Message>>& User::getMessages()const
    {
        return m_messages;
    }

    void User::JoinGroup(const std::shared_ptr<Group>& group)
    {
        if (!isInGroup(group->getGroupName()))
        {
            m_groups.push_back(group);
        }
    }

    void User::RemoveGroup(const std::shared_ptr<Group>& group)
    {
        m_groups.erase(std::remove_if(m_groups.begin(), m_groups.end(),
                [&group](const std::weak_ptr<Group>& g) {
                    return !g.owner_before(group) && !group.owner_before(g);
                }), m_groups.end());
    }

    bool User::isInGroup(const std::string& groupName) const
    {
        return std::any_of(m_groups.begin(), m_groups.end(), [&](const std::weak_ptr<Group>& g)
                {
                    if(auto shared = g.lock())
                    {
                        return shared->getGroupName() == groupName;
                    }
                    return false;
                });
    }

    void User::AddMessage(std::unique_ptr<Message> message)
    {
        m_messages.push_back(std::move(message));
    }
}
