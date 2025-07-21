#include "repositories/UserRepository.h"
#include "repositories/BaseRepository.h"
#include "datastore/IStore.h"
#include "domain/User.h"
#include "errorhandling/exceptions/UserNotFoundException.h"
using namespace ErrorHandling::Exceptions;

namespace Repository
{
    UserRepository::UserRepository(std::shared_ptr<DataSore::IStore<Domain::User>> store)
            :BaseRepository<Domain::User>(std::move(store)){}

    bool UserRepository::findUserByName(const std::string& username)
    {
        return ExistsById(username);
    }
    void UserRepository::disable(const std::string& username)
    {
        auto user = getById(username);
        if (!user)
            throw UserNotFoundException("DISABLE USER " + username, "User does not exist");

        user->disable();
    }
}