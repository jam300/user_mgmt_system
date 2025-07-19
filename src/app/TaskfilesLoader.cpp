#include "app/TaskFileLoader.h"

#include<fstream>
#include<iostream>
#include<filesystem>


using namespace TasksTypes;
namespace fs = std::filesystem;
namespace App
{
    /**
     * @brief Constructs a TaskFileLoader with the specified directory path.
     *This constructor initializes the loader to point to the directory containing task files.
     *
     * @param directoryPath_ The path to the directory where task files are located.
     */
    TaskFileLoader::TaskFileLoader(const std::string& directoryPath_)
        :m_directoryPath(directoryPath_){}
    
        /**
     * @brief Loads all task files from the specified directory.
     *
     * This function iterates over the files in the provided directory path and reads all `.txt` files
     * that are not empty and contain valid content (ignoring lines that are entirely whitespace).
     * Each valid file is parsed into a list of lines and returned as a pair of filename and content.
     *
     * @return ListOfTaskFiles A list of (filename, lines) pairs, each representing a parsed task file.
     *         If the directory doesn't exist or cannot be accessed, an empty list is returned.
     */
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