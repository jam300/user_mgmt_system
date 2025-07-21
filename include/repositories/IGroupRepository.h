#pragma once
#include "IRepository.h"
#include "domain/User.h"
#include <string>
#include <memory>

namespace Repository
{
    class IGroupRepository
    {
        public:
            virtual bool findGroupByName(const std::string& username) = 0;
            virtual void addUserToGrop(const std::string& groupname, const std::shared_ptr<Domain::User>& user) = 0;
            virtual void removeUserFromGroup(const std::string& groupname, const std::shared_ptr<Domain::User>& user) = 0;
            virtual bool IsGroupEmpty(const std::string& groupName) = 0;
    };
}