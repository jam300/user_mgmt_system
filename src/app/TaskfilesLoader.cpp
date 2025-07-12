#include "app/TaskFileLoader.h"

#include<fstream>
#include<iostream>
#include<filesystem>


using namespace TasksTypes;
namespace fs = std::filesystem;
namespace App
{
    TaskFileLoader::TaskFileLoader(const std::string& directoryPath_)
        :m_directoryPath(directoryPath_){}

    ListOfTaskFiles TaskFileLoader::LoadAllTasks()const
    {
        ListOfTaskFiles tasks;
        try
        {
            if(!fs::exists(m_directoryPath) || !fs::is_directory(m_directoryPath))
            {
                std::cerr << "[ERROR] Directory not found: " << m_directoryPath << std::endl;
                return tasks;
            }

            for(const auto& entry : fs::directory_iterator(m_directoryPath))
            {
                if(!entry.is_regular_file() || entry.path().extension() != ".txt") continue;

                std::ifstream file(entry.path());
                if(!file.is_open())
                {
                    std::cerr << "[WARNING] Could not open file: " << entry.path() << std::endl;
                    continue;
                }

                std::vector<std::string> lines;
                std::string line;
                while(std::getline(file, line))
                {
                    if(!line.empty() && line.find_first_not_of(" \t\r\n") != std::string::npos)
                    {
                        lines.push_back(line);
                    }
                }

                tasks.emplace_back(entry.path().filename().string(), std::move(lines));
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "[EXCEPTION] While loading tasks: " << e.what() << std::endl;
        }

        return tasks;
    }
}