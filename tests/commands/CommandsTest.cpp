#include <gtest/gtest.h>
#include "commands/AddUserToGroupCommand.h"
#include "commands/CreateUserCommand.h"
#include "commands/DelateUserCommand.h"
#include "commands/DisableUserCommand.h"
#include "commands/GetGroupsCommand.h"
#include "commands/GetUsersCommand.h"
#include "commands/PingCommand.h"
#include "commands/RemoveUserFromGroupCommand.h"
#include "commands/SendMessageCommand.h"
#include "domain/SystemState.h"
#include "domain/User.h"
#include "domain/Group.h"
#include "domain/Message.h"
#include "errorhandling/exceptions/AllExceptions.h"
#include "utils/Types.h"

using namespace Domain;
using namespace Commands;
using namespace ErrorHandling::Exceptions;

TEST(CreateUserCommandTest, AddsUserSuccessfully)
{
    SystemState state;

    std::string username = "alice";
    CreateUserCommand cmd(username);

    cmd.execute(state);

    EXPECT_EQ(state.getUsers().size(), 1);
    EXPECT_EQ(state.getUsers()[0]->getUsername(), "alice");
}

TEST(CreateUserCommandTest, FailedWhenAddAexistUser)
{
    SystemState state;

    std::string username = "alice";
    CreateUserCommand cmd(username);

    cmd.execute(state);

    CreateUserCommand cmd2(username);

    EXPECT_THROW(cmd2.execute(state), UserAlreadyExistsException);

}

TEST(DeleteUserCommandTest, DeletesUserSuccessfully)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    DeleteUserCommand cmd("alice");
    cmd.execute(state);

    EXPECT_FALSE(state.isUserExists("alice"));
}

TEST(DeleteUserCommandTest, ThrowsWhenUserDoesNotExist)
{
    SystemState state;
    DeleteUserCommand cmd("bob");

    EXPECT_THROW(cmd.execute(state), UserNotFoundException);
}
TEST(DisableUserCommandTest, DisablesUserSuccessfully)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    DisableUserCommand cmd("alice");
    cmd.execute(state);

    EXPECT_TRUE(user->isDisabled());
}

TEST(DisableUserCommandTest, ThrowsWhenUserDoesNotExist)
{
    SystemState state;
    DisableUserCommand cmd("ghost");

    EXPECT_THROW(cmd.execute(state),  UserNotFoundException);
}

TEST(SendMessageCommandTest, SendsMessageSuccessfully)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    SendMessageCommand cmd("alice", "Hello there!");
    cmd.execute(state);

    const auto& messages = user->getMessages();
    ASSERT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0]->getContent(), "Hello there!");
}

TEST(SendMessageCommandTest, ThrowsIfUserDoesNotExist)
{
    SystemState state;
    SendMessageCommand cmd("ghost", "Hi!");

    EXPECT_THROW(cmd.execute(state), UserNotFoundException);
}

TEST(SendMessageCommandTest, ThrowsIfUserIsDisabled)
{
    SystemState state;
    auto user = std::make_shared<User>("bob");
    user->disable();
    state.AddUser(user);

    SendMessageCommand cmd("bob", "Hi!");

    EXPECT_THROW(cmd.execute(state), CommandExecutionException);
}

TEST(AddUserToGroupCommandTest, AddsUserToNewGroup)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    AddUserToGroupCommand cmd("alice", "group1");
    cmd.execute(state);

    const auto& groups = state.getGroups();
    ASSERT_EQ(groups.size(), 1);
    EXPECT_EQ(groups[0]->getGroupName(), "group1");
    EXPECT_TRUE(groups[0]->hasMember("alice"));
}

TEST(AddUserToGroupCommandTest, ThrowIfUserAlreadyBelongsInThatGroup)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    AddUserToGroupCommand cmd("alice", "group1");
    cmd.execute(state);
    AddUserToGroupCommand cmd2("alice", "group1");

    EXPECT_THROW(cmd2.execute(state);, CommandExecutionException);
}

TEST(AddUserToGroupCommandTest, ThrowIfAddUserToGroupIfUSerDoenstExits)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    AddUserToGroupCommand cmd("alice", "group1");
    cmd.execute(state);
    AddUserToGroupCommand cmd2("Javier", "group1");

    EXPECT_THROW(cmd2.execute(state);, UserNotFoundException);
}

TEST(AddUserToGroupCommandTest, ThrowsIfUserIsDisabled)
{
    SystemState state;
    auto user = std::make_shared<User>("bob");
    user->disable();
    state.AddUser(user);

    AddUserToGroupCommand cmd("bob", "groupX");
    EXPECT_THROW(cmd.execute(state), CommandExecutionException);
}

TEST(RemoveUserFromGroupCommandTest, RemovesUserSuccessfully)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);
    auto user2 = std::make_shared<User>("javi");
    state.AddUser(user2);

    AddUserToGroupCommand addCmd("alice", "group1");
    addCmd.execute(state);

    AddUserToGroupCommand add2Cmd("javi", "group1");
    add2Cmd.execute(state);

    RemoveUserFromGroupCommand removeCmd("alice", "group1");
    removeCmd.execute(state);

    EXPECT_FALSE(state.getGroups()[0]->hasMember("alice"));
}

TEST(RemoveUserFromGroupCommandTest, ThrowIfRemovesUserInGroupThatDoesntBelong)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);
    auto user2 = std::make_shared<User>("jack");
    state.AddUser(user2);

    AddUserToGroupCommand addCmd("alice", "group1");
    addCmd.execute(state);

    RemoveUserFromGroupCommand removeCmd("jack", "group1");
    EXPECT_THROW(removeCmd.execute(state);, CommandExecutionException);
}

TEST(RemoveUserFromGroupCommandTest, ThrowIfRemovesUserThatDoesntExits)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    AddUserToGroupCommand addCmd("alice", "group1");
    addCmd.execute(state);

    RemoveUserFromGroupCommand removeCmd("jack", "group1");
    EXPECT_THROW(removeCmd.execute(state);, UserNotFoundException);
}

TEST(RemoveUserFromGroupCommandTest, ThrowIfRemovesUserFromGroupThatDoesntExits)
{
    SystemState state;
    auto user = std::make_shared<User>("alice");
    state.AddUser(user);

    AddUserToGroupCommand addCmd("alice", "group1");
    addCmd.execute(state);

    RemoveUserFromGroupCommand removeCmd("alice", "group12");
    EXPECT_THROW(removeCmd.execute(state);, CommandExecutionException);
}

TEST(RemoveUserFromGroupCommandTest, DeletesGroupWhenEmpty)
{
    SystemState state;
    auto user = std::make_shared<User>("bob");
    state.AddUser(user);

    AddUserToGroupCommand addCmd("bob", "group1");
    addCmd.execute(state);

    RemoveUserFromGroupCommand removeCmd("bob", "group1");
    removeCmd.execute(state);

    EXPECT_TRUE(state.getGroups().empty());
}

TEST(GetUsersCommandTest, PrintsAllUsers)
{
    SystemState state;
    state.AddUser(std::make_shared<User>("alice"));
    state.AddUser(std::make_shared<User>("bob"));

    GetUsersCommand cmd;
    EXPECT_NO_THROW(cmd.execute(state));
}

TEST(GetUsersCommandTest, PrintsUserListToOutput)
{

    SystemState state;
    state.AddUser(std::make_shared<User>("alice"));
    state.AddUser(std::make_shared<User>("bob"));


    std::stringstream buffer;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());


    GetUsersCommand cmd;
    cmd.execute(state);


    std::cout.rdbuf(original_cout);


    std::string output = buffer.str();

    EXPECT_NE(output.find("GET USERS"), std::string::npos);
    EXPECT_NE(output.find("alice"), std::string::npos);
    EXPECT_NE(output.find("bob"), std::string::npos);
}

TEST(GetGroupsCommandTest, PrintsAllGroups)
{
    SystemState state;
    state.AddUser(std::make_shared<User>("alice"));

    AddUserToGroupCommand addCmd("alice", "devs");
    addCmd.execute(state);

    GetGroupsCommand cmd;
    EXPECT_NO_THROW(cmd.execute(state));
}

TEST(GetGroupsCommandTest, PrintsGroupListToOutput)
{
    SystemState state;

    auto user = std::make_shared<User>("javi");
    state.AddUser(user);

    AddUserToGroupCommand addCmd("javi", "developers");
    addCmd.execute(state);

    std::stringstream buffer;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());


    GetGroupsCommand cmd;
    cmd.execute(state);

    std::cout.rdbuf(original_cout);

    std::string output = buffer.str();
    EXPECT_NE(output.find("GET GROUPS"), std::string::npos);
    EXPECT_NE(output.find("developers"), std::string::npos);
}

TEST(PingCommandTest, PrintsCorrectlyToExistingUser)
{
    SystemState state;
    state.AddUser(std::make_shared<User>("javi"));

    std::stringstream buffer;
    std::streambuf* original = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    PingCommand cmd("javi", "2");
    cmd.execute(state);

    std::cout.rdbuf(original);

    std::string output = buffer.str();
    EXPECT_NE(output.find("Send Ping to javi (2)"), std::string::npos);
    EXPECT_EQ(std::count(output.begin(), output.end(), '\n'), 5);
    EXPECT_NE(output.find("javi received a ping"), std::string::npos);
}

TEST(PingCommandTest, PrintsCorrectlyToNonExistentUser)
{
    SystemState state;

    std::stringstream buffer;
    std::streambuf* original = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    PingCommand cmd("ghost", "3");
    cmd.execute(state);

    std::cout.rdbuf(original);

    std::string output = buffer.str();
    EXPECT_NE(output.find("Send Ping to ghost (3)"), std::string::npos);
    EXPECT_EQ(std::count(output.begin(), output.end(), '\n'), 4); // encabezado + 3 pings
    EXPECT_EQ(output.find("ghost received a ping"), std::string::npos); // no debe imprimirse
}

TEST(PingCommandTest, ThrowsWhenArgumentIsInvalid)
{
    EXPECT_THROW(PingCommand("javi", "invalid"), InvalidArgumentException);
}