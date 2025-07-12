#include "domain/SystemState.h"
#include "errorhandling/exceptions/AllExceptions.h"

#include <iterator>
#include <vector>
#include <memory>

using namespace ErrorHandling::Exceptions;

namespace Domain
{
    bool SystemState::isUserExists(const std::string& username) const
    {
        return m_userMap.find(username) != m_userMap.end();
    }

    bool SystemState::isGroupExists(const std::string& groupName) const
    {
        return m_groupMap.find(groupName) != m_groupMap.end();
    }

    bool SystemState::isUserInGroup(const std::string& username, const std::string& groupName) const
    {
        auto groupIt = m_groupMap.find(groupName);
        if (groupIt == m_groupMap.end())
            return false;

        const auto& users = groupIt->second->getMembers(); // Devuelve vector<weak_ptr<User>>
        for (const auto& user : users)
        {
            if (user && user->getUsername() == username)
                return true;
        }
        return false;
    }

    void SystemState::AddUser(const std::shared_ptr<User>& user)
    {
        if (isUserExists(user->getUsername()))
        {
            throw  UserAlreadyExistsException("ADD USER ", "User " + user->getUsername() + " already exist");
        }

        m_userMap[user->getUsername()] = user;
    }

    void SystemState::CreateNewGroup(const std::shared_ptr<Group>& group)
    {
        m_groupMap[group->getGroupName()] = group;
    }

    void SystemState::DeleteUser(const std::string& username)
    {
        if (!isUserExists(username))
        {
            throw  UserNotFoundException("DELETE USER", "User: " + username + " does not exist");
        }

        m_userMap.erase(username);
    }

    void SystemState::DisableUser(const std::string& username)
    {
        if (!isUserExists(username))
        {
            throw  UserNotFoundException("DISABLE USER " + username , "User does not exist");
        }

        m_userMap[username]->disable();
    }

    std::vector<std::shared_ptr<User>> SystemState::getUsers() const
    {
        std::vector<std::shared_ptr<User>> users;
        users.reserve(m_userMap.size());

        std::transform(m_userMap.begin(), m_userMap.end(), std::back_inserter(users),
                    [](const auto& pair) { return pair.second; });

        return users;
    }

    std::vector<std::shared_ptr<Group>> SystemState::getGroups() const
    {
        std::vector<std::shared_ptr<Group>> groups;
        groups.reserve(m_groupMap.size());

        std::transform(m_groupMap.begin(), m_groupMap.end(), std::back_inserter(groups),
                    [](const auto& pair) { return pair.second; });

        return groups;
    }

    void SystemState::AddUserToGroup(const std::string& username, const std::string& groupName)
    {
        if (!isUserExists(username))
            throw UserNotFoundException("ADD USER " + username + " TO GROUP " + groupName, "User does not exist");

        auto user = m_userMap.at(username);

        if(user->isDisabled())
            throw CommandExecutionException("ADD USER " + username + " TO GROUP " + groupName, "User is disabled");

        if (!isGroupExists(groupName))
        {
            auto newGroup = std::make_shared<Group>(groupName);
            CreateNewGroup(newGroup);
        }

        auto group = m_groupMap.at(groupName);
        group->AddMembers(user);
    }

    void SystemState::RemoveUserFromGroup(const std::string& username, const std::string& groupName)
    {
        if (!isUserExists(username) )
            throw UserNotFoundException("REMOVE USER " + username + " TO GROUP " + groupName, "User does not exist");

        if ( !isGroupExists(groupName))
            throw UserNotFoundException("REMOVE USER " + username + " TO GROUP " + groupName, "Group does not exist");

        if ( !isUserInGroup(username, groupName))
            throw UserNotFoundException("REMOVE USER " + username + " TO GROUP " + groupName, "User doesn't belong in that group");

        auto user = m_userMap.at(username);
        auto group = m_groupMap.at(groupName);

        group->RemoveMember(user);
        user->RemoveGroup(group);

        if (group->getMemberCount() == 0)
        {
            m_groupMap.erase(groupName);
        }
    }

    void SystemState::SendMessage(const std::string& toUser, std::unique_ptr<Message> message)
    {
        if (!isUserExists(toUser))
        {
            throw UserNotFoundException("SEND MESSAGE  " + toUser + " '" + message->getContent() +" '", "User does not exist");
        }

        auto user = m_userMap.at(toUser);
        if(user->isDisabled())
            throw CommandExecutionException("SEND MESSAGE  " + toUser + " '" + message->getContent() +" '", "User is disabled");
        user->AddMessage(std::move(message));
    }

    const std::vector<std::unique_ptr<Message>>& SystemState::getMessageHistory(const std::string& username) const
    {
        if (!isUserExists(username))
        {
            throw UserNotFoundException("GET MESSAGE HISTORY " + username, "User does not exist");
        }

        return m_userMap.at(username)->getMessages();
    }


}