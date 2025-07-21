#pragma once

#include <memory>
#include <string>
#include <vector>

namespace DataSore
{
    template<typename T>
    class IStore
    {
        virtual void save(const std::shared_ptr<T>& entity) = 0;
        virtual void Remove(const std::string& id) = 0;
        virutal std::shared_ptr<T> GetById(const std::string& id) = const = 0;
        virtual std::vector<std::shared_ptr<T>> GetAll() const = 0;
        virtual bool Contains(const std::string& name) const = 0;
    };
}