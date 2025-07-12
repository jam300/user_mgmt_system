#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <optional>

#include "User.h"
#include "Group.h"
#include "Message.h"

namespace Domain
{
    class SystemState
    {
        public:
            SystemState() = default;
            SystemState(const SystemState&) = delete;
            SystemState& operator=(const SystemState&) = delete;
            SystemState(SystemState&&) noexcept = default;
            SystemState& operator=(SystemState&&) noexcept = default;
            ~SystemState() = default;

            void AddUser(const std::shared_ptr<User>& user);
            bool isUserExists(const std::string& username) const;
            void DeleteUser(const std::string& username);
            void DisableUser(const std::string& username);
            std::vector<std::shared_ptr<User>> getUsers() const;
            std::vector<std::shared_ptr<Group>> getGroups() const;

            void AddUserToGroup(const std::string& username, const std::string& groupName);
            void RemoveUserFromGroup(const std::string& username, const std::string& groupName);

            void SendMessage(const std::string& toUser, std::unique_ptr<Message> message);
            const std::vector<std::unique_ptr<Message>>& getMessageHistory(const std::string& username) const;

        private:
            bool isGroupExists(const std::string& groupName) const;
            bool isUserInGroup(const std::string& username, const std::string& groupName) const;
            void CreateNewGroup(const std::shared_ptr<Group>& group);

            std::unordered_map<std::string, std::shared_ptr<User>> m_userMap;
            std::unordered_map<std::string, std::shared_ptr<Group>> m_groupMap;
    };
}