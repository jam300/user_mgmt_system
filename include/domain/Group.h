#pragma once

#include<string>
#include<vector>
#include<memory>

namespace Domain
{
    class User;
    class Group: public std::enable_shared_from_this<Group>
    {
        public:
            explicit Group(std::string groupName_);

            const std::string& getGroupName() const;
            const std::vector<std::shared_ptr<User>>& getMembers() const;

            void AddMembers(const std::shared_ptr<User>& user);
            void RemoveMember(const std::shared_ptr<User>& user);
            bool hasMember(const std::string& username) const;
            int getMemberCount() const;

        private:
            int m_memberCount = 0;
            std::string m_groupName;
            std::vector<std::shared_ptr<User>> m_groupMembers;
    };
}
