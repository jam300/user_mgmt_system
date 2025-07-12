#pragma once

#include<string>
#include<vector>
#include<algorithm>
#include<memory>

namespace Domain
{
    class Group;
    class Message;

    class User: public std::enable_shared_from_this<User>
    {
        public:
            explicit User(std::string username_);

            ~User() = default;

            const std::string& getUsername() const;
            bool isDisabled() const;
            void disable();
            const std::vector<std::weak_ptr<Group>>& getGroups() const;
            const std::vector<std::unique_ptr<Message>>& getMessages() const;

            void JoinGroup(const std::shared_ptr<Group>& group);
            void RemoveGroup(const std::shared_ptr<Group>& group);
            bool isInGroup(const std::string& group) const;
            void AddMessage(std::unique_ptr<Message> message);

        private:
            std::string m_userName;
            bool m_disable = false;
            std::vector<std::weak_ptr<Group>> m_groups;
            std::vector<std::unique_ptr<Message>> m_messages;
    };
}