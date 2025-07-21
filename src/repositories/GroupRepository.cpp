#include "repositories/GroupRepository.h"
#include "repositories/BaseRepository.h"
#include "datastore/IStore.h"
#include "domain/Group.h"
#include "errorhandling/exceptions/UserNotFoundException.h"
using namespace ErrorHandling::Exceptions;

namespace Repository
{
    GroupRepository::GroupRepository(std::shared_ptr<DataSore::IStore<Domain::Group>> store)
            :BaseRepository<Domain::Group>(std::move(store)){}

    bool GroupRepository::findGroupByName(const std::string& username)
    {
        return ExistsById(username);
    }

    void GroupRepository::addUserToGrop(const std::string& groupname, const std::shared_ptr<Domain::User>& user)
    {
        auto group = getById(groupname);
        group->AddMembers(user);
        Add(group);

    }

    void GroupRepository::removeUserFromGroup(const std::string& groupname, const std::shared_ptr<Domain::User>& user)
    {
        auto group = getById(groupname);
        group->RemoveMember(user);
        Add(group);
    }

    bool GroupRepository::IsGroupEmpty(const std::string& groupName)
    {
        auto group = getById(groupName);
        return group && group->getMemberCount() == 0;
    }
}