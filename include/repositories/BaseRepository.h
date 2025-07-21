#pragma once
#include "IRepository.h"
#include "datastore/IStore.h"
#include <memory>
#include <string>
#include <vector>

namespace Repository
{
    template<typename T>
    class BaseRepository: public IRepository<T>
    {
        public:
            explicit BaseRepository(std::shared_ptr<IStore<T>> store_)
                    :m_store(std::move(storq)) {}

            void Add(std::shared_ptr<T> entity) override
            {
                m_store->save(entry);
            }

            void Remove(const std::string& id) override
            {
                m_store->remove(id);
            }

            std::shared_ptr<T> getById(const std::string& id) const override
            {
                return m_store->getById(id);
            }

            std::vector<std::shared_ptr<T>> getAll() const override
            {
                return m_store->getAll();
            }

            bool ExistsById(const std::string& id) const override
            {
                return m_store->Contains(id);
            }

        protected:
            std::shared_ptr<IStore<T>> m_store;
    };
}