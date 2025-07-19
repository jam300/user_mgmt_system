#include "domain/SystemState.h"
#include "errorhandling/exceptions/AllExceptions.h"

#include <iterator>
#include <vector>
#include <memory>

using namespace ErrorHandling::Exceptions;

namespace Domain
{
    /**
     * @brief Checks if a user with the given username exists.
     * @param username The username to check.
     * @return True if the user exists, false otherwise.
     */
    bool SystemState::isUserExists(const std::string& username) const
    {
        return m_userMap.find(username) != m_userMap.end();
    }
    /**
     * @brief Checks if a group with the given name exists.
     * @param groupName The group name to check.
     * @return True if the group exists, false otherwise.
     */
    bool SystemState::isGroupExists(const std::string& groupName) const
    {
        return m_groupMap.find(groupName) != m_groupMap.end();
    }
    /**
     * @brief Checks if a user belongs to a specific group.
     * @param username The username to check.
     * @param groupName The group to verify membership.
     * @return True if the user belongs to the group, false otherwise.
     */
    bool SystemState::isUserInGroup(const std::string& username, const std::string& groupName) const
    {
        auto groupIt = m_groupMap.find(groupName);
        if (groupIt == m_groupMap.end())
            return false;

        const auto& users = groupIt->second->getMembers();
        for (const auto& user: users)
        {
            if (user && user->getUsername() == username)
                return true;
        }
        return false;
    }
    /**
     * @brief Adds a new user to the system.
     * @param user Shared pointer to the User object.
     * @throws UserAlreadyExistsException if the user already exists.
     */
    void SystemState::AddUser(const std::shared_ptr<User>& user)
    {
        if (isUserExists(user->getUsername()))
            throw  UserAlreadyExistsException("ADD USER ", "User " + user->getUsername() + " already exist");

        m_userMap[user->getUsername()] = user;
    }
    /**
     * @brief Creates a new group in the system.
     * @param group Shared pointer to the Group object.
     */
    void SystemState::CreateNewGroup(const std::shared_ptr<Group>& group)
    {
        m_groupMap[group->getGroupName()] = group;
    }
    /**
     * @brief Deletes a user from the system.
     * @param username The username of the user to delete.
     * @throws UserNotFoundException if the user does not exist.
     */
    void SystemState::DeleteUser(const std::string& username)
    {
        if (!isUserExists(username))
            throw  UserNotFoundException("DELETE USER", "User: " + username + " does not exist");

        m_userMap.erase(username);
    }
    /**
     * @brief Disables a user in the system (soft removal).
     * @param username The username of the user to disable.
     * @throws UserNotFoundException if the user does not exist.
     */
    void SystemState::DisableUser(const std::string& username)
    {
        if (!isUserExists(username))
        {
            throw  UserNotFoundException("DISABLE USER " + username, " User does not exist");
        }

        m_userMap[username]->disable();
    }
    /**
     * @brief Retrieves all users in the system.
     * @return Vector of shared pointers to all users.
     */
    std::vector<std::shared_ptr<User>> SystemState::getUsers() const
    {
        std::vector<std::shared_ptr<User>> users;
        users.reserve(m_userMap.size());

        std::transform(m_userMap.begin(), m_userMap.end(), std::back_inserter(users),
                    [](const auto& pair) { return pair.second; });

        return users;
    }
    /**
     * @brief Retrieves all groups in the system.
     * @return Vector of shared pointers to all groups.
     */
    std::vector<std::shared_ptr<Group>> SystemState::getGroups() const
    {
        std::vector<std::shared_ptr<Group>> groups;
        groups.reserve(m_groupMap.size());

        std::transform(m_groupMap.begin(), m_groupMap.end(), std::back_inserter(groups),
                    [](const auto& pair) { return pair.second; });

        return groups;
    }
    /**
     * @brief Adds a user to a group. If the group doesn't exist, it will be created.
     * @param username The name of the user to add.
     * @param groupName The name of the group.
     * @throws UserNotFoundException if the user does not exist.
     * @throws CommandExecutionException if the user is already in the group or is disabled.
     */
    void SystemState::AddUserToGroup(const std::string& username, const std::string& groupName)
    {
        if (!isUserExists(username))
            throw  UserNotFoundException("ADD USER " + username + " TO GROUP " + groupName, " User does not exist");

        if (isUserInGroup(username, groupName))
            throw CommandExecutionException("ADD USER " + username + " TO GROUP " + groupName, " User already belong in that group");

        auto user = m_userMap.at(username);

        if(user->isDisabled())
            throw CommandExecutionException("ADD USER " + username + " TO GROUP " + groupName, " User is disabled");

        if (!isGroupExists(groupName))
        {
            auto newGroup = std::make_shared<Group>(groupName);
            CreateNewGroup(newGroup);
        }

        auto group = m_groupMap.at(groupName);
        group->AddMembers(user);
    }
    /**
     * @brief Removes a user from a group. If the group becomes empty, it is deleted.
     * @param username The user to remove.
     * @param groupName The group to remove the user from.
     * @throws UserNotFoundException if the user does not exist.
     * @throws CommandExecutionException if the group doesn't exist or the user isn't in it.
     */
    void SystemState::RemoveUserFromGroup(const std::string& username, const std::string& groupName)
    {
        if (!isUserExists(username) )
            throw UserNotFoundException("REMOVE USER " + username + " FROM GROUP " + groupName, " User does not exist");

        if ( !isGroupExists(groupName))
            throw CommandExecutionException("REMOVE USER " + username + " FROM GROUP " + groupName, " Group does not exist");

        if ( !isUserInGroup(username, groupName))
            throw CommandExecutionException("REMOVE USER " + username + " FROM GROUP " + groupName, " User doesn't belong in that group");

        auto user = m_userMap.at(username);
        auto group = m_groupMap.at(groupName);

        group->RemoveMember(user);
        user->RemoveGroup(group);

        if (group->getMemberCount() == 0)
                m_groupMap.erase(groupName);
    }
    /**
     * @brief Sends a message to a specific user.
     * @param toUser The recipient username.
     * @param message The message to send.
     * @throws UserNotFoundException if the recipient doesn't exist.
     * @throws CommandExecutionException if the user is disabled.
     */
    void SystemState::SendMessage(const std::string& toUser, std::unique_ptr<Message> message)
    {
        if (!isUserExists(toUser))
            throw UserNotFoundException("SEND MESSAGE  " + toUser + " '" + message->getContent() +" '", " User does not exist");

        auto user = m_userMap.at(toUser);
        if(user->isDisabled())
            throw CommandExecutionException("SEND MESSAGE  " + toUser + " '" + message->getContent() +" '", " User is disabled");
        user->AddMessage(std::move(message));
    }
    /**
     * @brief Retrieves the message history of a user.
     * @param username The user whose message history to retrieve.
     * @return Reference to the vector of messages.
     * @throws UserNotFoundException if the user does not exist.
     */
    const std::vector<std::unique_ptr<Message>>& SystemState::getMessageHistory(const std::string& username) const
    {
        if (!isUserExists(username))
            throw UserNotFoundException("GET MESSAGE HISTORY " + username, " User does not exist");

        return m_userMap.at(username)->getMessages();
    }


}
