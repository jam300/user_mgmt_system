#include "domain/User.h"
#include "domain/Group.h"
#include "domain/Message.h"
#include <algorithm>

namespace Domain
{
    /**
    * @brief Constructs a User with the given username.
    * @param username_ The username for the user.
    */
    User::User(std::string username_):m_userName(std::move(username_)){}
    /**
    * @brief Retrieves the username of the user.
    * @return A const reference to the username string.
    */
    const std::string& User::getUsername() const
    {
        return m_userName;
    }
    /**
    * @brief Checks if the user is disabled.
    * @return True if the user is disabled, false otherwise.
    */
    bool User::isDisabled() const
    {
        return m_disable;
    }
    /**
    * @brief Disables the user.
    */
    void User::disable()
    {
        m_disable = true;
    }
    /**
    * @brief Gets the list of groups the user is a member of.
    * @return A const reference to a vector of weak pointers to groups.
    */
    const std::vector<std::weak_ptr<Group>>& User::getGroups()const
    {
        return m_groups;
    }
    /**
    * @brief Gets the list of messages received by the user.
    * @return A const reference to a vector of unique pointers to messages.
    */
    const std::vector<std::unique_ptr<Message>>& User::getMessages()const
    {
        return m_messages;
    }
    /**
    * @brief Adds the user to the specified group.
    * @param group A shared pointer to the group to join.
    */
    void User::JoinGroup(const std::shared_ptr<Group>& group)
    {
        if (!isInGroup(group->getGroupName()))
        {
            m_groups.push_back(group);
        }
    }
    /**
    * @brief Removes the user from the specified group.
    * @param group A shared pointer to the group to leave.
    */
    void User::RemoveGroup(const std::shared_ptr<Group>& group)
    {
        m_groups.erase(std::remove_if(m_groups.begin(), m_groups.end(),
                [&group](const std::weak_ptr<Group>& g) {
                    return !g.owner_before(group) && !group.owner_before(g);
                }), m_groups.end());
    }
    /**
    * @brief Checks whether the user is in a given group.
    * @param group The name of the group to check.
    * @return True if the user is in the group, false otherwise.
    */
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
    /**
    * @brief Adds a message to the user's message list.
    * @param message A unique pointer to the message to add.
    */
    void User::AddMessage(std::unique_ptr<Message> message)
    {
        m_messages.push_back(std::move(message));
    }
}
