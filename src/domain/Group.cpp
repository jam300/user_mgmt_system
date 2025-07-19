#include "domain/Group.h"
#include "domain/User.h"
#include <algorithm>

namespace Domain
{
    /**
     * @brief Constructs a Group with the given group name.
     * @param groupName_ The name of the group.
     */
    Group::Group(std::string groupName_):m_groupName(std::move(groupName_)){}
    /**
     * @brief Gets the name of the group.
     * @return const std::string& The name of the group.
     */
    const std::string& Group::getGroupName() const
    {
        return m_groupName;
    }
    /**
     * @brief Gets the members of the group.
     * @return const std::vector<std::shared_ptr<User>>& A vector containing shared pointers to the users in the group.
     */
    const std::vector<std::shared_ptr<User>>& Group::getMembers() const
    {
        return m_groupMembers;
    }
    /**
     * @brief Adds a user to the group if they are not already a member.
     * @param user A shared pointer to the user to be added.
     */
    void Group::AddMembers(const std::shared_ptr<User>& user)
    {
        if(!hasMember(user->getUsername()))
        {
            m_groupMembers.push_back(user);
            ++m_memberCount;
            user->JoinGroup(shared_from_this());
        }
    }
    /**
     * @brief Removes a user from the group.
     * @param user A shared pointer to the user to be removed.
     */
    void Group::RemoveMember(const std::shared_ptr<User>& user)
    {
        m_groupMembers.erase(std::remove_if(m_groupMembers.begin(), m_groupMembers.end(),
                    [&](const std::shared_ptr<User>& u) {
                        return u && u->getUsername() == user->getUsername();
                    }), m_groupMembers.end());

        --m_memberCount;
        user->RemoveGroup(shared_from_this());
    }
    /**
     * @brief Checks whether a user with the given username is a member of the group.
     * @param username The username to check.
     * @return true If the user is a member.
     * @return false If the user is not a member.
     */
    bool Group::hasMember(const std::string& username) const
    {
        return std::any_of(m_groupMembers.begin(), m_groupMembers.end(),
            [&](const std::shared_ptr<User>& user) {
                return user && user->getUsername() == username;
            });
    }
    /**
     * @brief Gets the number of members in the group.
     * @return int The number of members.
     */
    int Group::getMemberCount() const { return m_memberCount; }

}