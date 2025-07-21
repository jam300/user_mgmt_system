#pragma once

#include "domain/User.h".
#include "repositories/BaseRepository.h"
#include "repositories/IUserRepository.h"
#include "datastore/IStore.h"

namespace Repository
{
    class UserRepository : public BaseRepository<Domain::User>, public IUserRepository
    {
        public:
            explicit UserRepository(std::shared_ptr<DataSore::IStore<Domain::User>> store);

            bool findUserByName(const std::string& username) override;
            void disable(const std::string& username) override;
    };
}