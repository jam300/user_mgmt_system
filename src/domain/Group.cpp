#include "domain/Group.h"
#include "domain/User.h"
#include <algorithm>

namespace Domain
{
    Group::Group(std::string groupName_):m_groupName(std::move(groupName_)){}

    const std::string& Group::getGroupName() const
    {
        return m_groupName;
    }

    const std::vector<std::shared_ptr<User>>& Group::getMembers() const
    {
        return m_groupMembers;
    }

    void Group::AddMembers(const std::shared_ptr<User>& user)
    {
        if(!hasMember(user->getUsername()))
        {
            m_groupMembers.push_back(user);
            ++m_memberCount;
            user->JoinGroup(shared_from_this());
        }
    }

    void Group::RemoveMember(const std::shared_ptr<User>& user)
    {
        m_groupMembers.erase(std::remove_if(m_groupMembers.begin(), m_groupMembers.end(),
                    [&](const std::shared_ptr<User>& u) {
                        return u && u->getUsername() == user->getUsername();
                    }), m_groupMembers.end());

        --m_memberCount;
        user->RemoveGroup(shared_from_this());
    }

    bool Group::hasMember(const std::string& username) const
    {
        return std::any_of(m_groupMembers.begin(), m_groupMembers.end(),
            [&](const std::shared_ptr<User>& user) {
                return user && user->getUsername() == username;
            });
    }
    int Group::getMemberCount() const { return m_memberCount; }

}