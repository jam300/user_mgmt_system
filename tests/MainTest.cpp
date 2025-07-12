#include <gtest/gtest.h>
#include "app/CommandRegistry.h"
#include "app/TasksParser.h"
#include "commands/CreateUserCommand.h"
#include "commands/SendMessageCommand.h"
#include "commands/AddUserToGroupCommand.h"
#include<memory>

TEST(SampleTest, BasicAssertion)
{
    EXPECT_EQ(1 + 1, 2);
}

TEST(CommandRegistryTest, CanParseMultipleCommandsFromSingleTaskFile)
{
        App::CommandRegistry registry;
    auto m_parser = std::make_unique<App::TasksParser>(registry);
    std::vector<std::string> lines = {
        "CREATE USER alice",
        "SEND MESSAGE alice \"Hello, alice!\"",
        "ADD USER alice TO GROUP admins"
    };

    auto parsedTasks = m_parser->ParseTasks("Tasks_test", lines);

    ASSERT_EQ(parsedTasks.size(), 1);


    EXPECT_EQ(parsedTasks[0].first, "Tasks_test");


    const TasksTypes::CommandList& commandsInFile = parsedTasks[0].second;


    ASSERT_EQ(commandsInFile.size(), 3);


    EXPECT_NE(dynamic_cast<Commands::CreateUserCommand*>(commandsInFile[0].get()), nullptr);
    EXPECT_NE(dynamic_cast<Commands::SendMessageCommand*>(commandsInFile[1].get()), nullptr);
    EXPECT_NE(dynamic_cast<Commands::AddUserToGroupCommand*>(commandsInFile[2].get()), nullptr);
}

TEST(CommandRegistryTest, TaskFailsIfAnyCommandFails)
{
    App::CommandRegistry registry;
    auto m_parser = std::make_unique<App::TasksParser>(registry);
    std::vector<std::string> lines = {
        "CREATE USER alice",
        "THIS IS AN INVALID COMMAND LINE",
        "ADD USER alice TO GROUP admins"
    };

    auto parsedTasks = m_parser->ParseTasks("Tasks_test_with_error", lines);

    ASSERT_EQ(parsedTasks.size(), 0);
}