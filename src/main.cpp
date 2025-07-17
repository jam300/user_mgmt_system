#include "app/TaskManager.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <functional>

namespace fs = std::filesystem;

void showMenu()
{
    std::cout << "\n=== MAIN MENU ===\n"
            << "1. Run tasks from files\n"
            << "2. Show task directory path\n"
            << "3. Update task directory path\n"
            << "4. Exit\n"
            << "Select an option: ";
}

int main()
{
    try
    {
        std::string relativePath = "../tasks";
        std::string tasksPath = fs::absolute(relativePath).string();
        std::unique_ptr<App::TaskManager> taskMan = std::make_unique<App::TaskManager>(tasksPath);
        auto systemState = std::make_shared<Domain::SystemState>();
        taskMan->SetState(systemState);

        bool running = true;

        std::map<char, std::function<void()>> menuOptions = {
            {'1', [&]() {
                std::cout << "🔄 Running tasks...\n";
                taskMan->RunTasksFromFiles();
            }},
            {'2', [&]() {
                std::cout << "📂 Task directory path: " << tasksPath << '\n';

            }},
            {'3', [&]() {
                std::cout << "📝 Enter new task directory path: ";
                std::string newPath;
                std::getline(std::cin >> std::ws, newPath);
                if (!newPath.empty()) {
                    std::string absolutePath = fs::absolute(newPath).string();
                    if (fs::exists(absolutePath)) {
                        tasksPath = absolutePath;
                        taskMan->UpdateTasksPath(tasksPath);
                        std::cout << "✅ Path updated to: " << tasksPath << "\n";
                    } else {
                        std::cout << "❌ Path does not exist: " << absolutePath << "\n";
                    }
                } else
                {
                    std::cout << "⚠️ Empty path. No changes made.\n";
                }
            }},
            {'4', [&]() {
                std::cout << "👋 Exiting program...\n";
                running = false;
            }}
        };

        while (running)
        {
            showMenu();

        char option;
        std::cin >> option;
        std::cin.ignore();

        if (auto it = menuOptions.find(option); it != menuOptions.end())
        {
            it->second();
        }
        else
        {
            std::cout << "⚠️ Opcion no valida.\n";
        }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "❌ Error al cargar tareas: " << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}