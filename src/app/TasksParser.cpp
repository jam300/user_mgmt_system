#include "App/TasksParser.h"
#include "errorhandling/ErrorHandler.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace ErrorHandling::Exceptions;
using namespace TasksTypes;
namespace App
{
    TasksParser::TasksParser(const CommandRegistry &registry)
                : m_registry(registry){}

    ParsedTasks TasksParser::ParseTasks(const std::string& fileName, const std::vector<std::string>& rawTasks) const
    {
        ParsedTasks parsed;
        CommandList commands;
        bool taskHasError = false;

        for (const auto& rawline : rawTasks)
        {
            std::string newLine = CleanLine(rawline);

            if (newLine.empty())
                continue;

            try
            {
                auto [commandName, args] = ExtractCommandAndArgs(newLine);
                auto command = m_registry.createCommand(commandName, args);
                commands.push_back(std::move(command));
            }
            catch (const BaseException& e)
            {
                ErrorHandler::Handle(e, "CreateCommandFromLine");
                taskHasError = true;
                break;
            }
        }

        if (!taskHasError)
            parsed.emplace_back(fileName, std::move(commands));

        return parsed;
    }

    std::string TasksParser::CleanLine(const std::string& line) const
    {

        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));

        if (trimmed.empty() || trimmed[0] == '#')
            return "";

        size_t commentPos = trimmed.find('#');
        if (commentPos != std::string::npos)
            trimmed = trimmed.substr(0, commentPos);


        trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
        return trimmed;
    }

    TaskFile TasksParser::ExtractCommandAndArgs(const std::string &line) const
    {
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string token;
        std::string quoted;

        while (iss >> std::ws)
        {
            if (iss.peek() == '"')
            {
                std::string quoted;
                iss.get(); // consume "
                std::getline(iss, quoted, '"');
                tokens.push_back(quoted);

                if (iss.peek() == ' ')
                    iss.get();
            }
            else
            {
                iss >> token;
                tokens.push_back(token);
            }
        }

        std::string commandName;
        for(const std::string& word: tokens)
        {
            if( std::all_of(word.begin(), word.end(), [](unsigned char c){
                            return  std::isupper(c);
                        }))
            {
                if (!commandName.empty())
                    commandName += " ";
                commandName += word;
            }
        }

        std::vector<std::string> args;
        std::copy_if(tokens.begin(), tokens.end(), std::back_inserter(args), [&](const std::string& word) {
            std::string pattern = " " + word + " ";
            std::string paddedCommand = " " + commandName + " ";
            return paddedCommand.find(pattern) == std::string::npos;
        });

        return  {commandName, args};
    }
}

