#pragma once
#include "utils/Types.h"
#include<string>
#include<vector>
#include<utility>

namespace App
{
    class TaskFileLoader
    {
        public:
            explicit TaskFileLoader(const std::string& directoryPath_);
            TasksTypes::ListOfTaskFiles LoadAllTasks() const;

        private:
            std::string m_directoryPath;
    };
}