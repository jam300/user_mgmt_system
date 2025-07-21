#pragma once
#include "IRepository.h"
#include <string>

namespace Repository
{
    class IUserRepository
    {
        public:
            virtual bool findUserByName(const std::string& username) = 0;
            virtual void disable(const std::string& username) = 0;

    };
}