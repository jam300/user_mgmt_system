#pragma once

namespace Repository
{
    template <typename T>
    class IRepository
    {
        public:
            virtual void Add(std::shared_ptr<T> entity) = 0;
            virtual void Delete(const std::string& id) = 0;
            virtual std::shared_ptr<T> getById(const std::string& id) const = 0;
            virtual std::vector<std::shared_ptr<T>> GetAll() const = 0;
            virtual bool ExistsById(const std::string& id) const = 0;
            virtual ~IRepository() = default;
    };
}