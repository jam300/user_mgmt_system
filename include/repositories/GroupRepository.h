#pragma once

#include "domain/Group.h".
#include "repositories/BaseRepository.h"
#include "repositories/IGroupRepository.h"
#include "datastore/IStore.h"

namespace Repository
{
    class GroupRepository : public BaseRepository<Domain::Group>, public IGroupRepository
    {
        public:
            explicit GroupRepository(std::shared_ptr<DataSore::IStore<Domain::Group>> store);

            bool findGroupByName(const std::string& username) override;
            void addUserToGrop(const std::string& groupname, const std::shared_ptr<Domain::User>& user) override;
            void removeUserFromGroup(const std::string& groupname, const std::shared_ptr<Domain::User>& user) override;
            bool IsGroupEmpty(const std::string& groupName) override;
    };
}