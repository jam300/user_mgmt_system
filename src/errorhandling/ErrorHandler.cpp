#include "errorhandling/ErrorHandler.h"
#include "commandresult/OutputPrinter.h"

#include <iostream>
using CommandResult::OutputPrinter;
namespace ErrorHandling::Exceptions
{
    static ErrorHandler::HandlerFunc BaseHandler = [](const BaseException& e)
    {
        OutputPrinter::PrintCommandFailure(e.GetCommandLine(), e.GetFailureReason());
    };

    const std::unordered_map<std::type_index, ErrorHandler::HandlerFunc> ErrorHandler::handlers_ = {
        { std::type_index(typeid(InvalidCommandException)),     BaseHandler },
        { std::type_index(typeid(UserAlreadyExistsException)),  BaseHandler },
        { std::type_index(typeid(UserNotFoundException)),       BaseHandler },
        { std::type_index(typeid(InvalidArgumentException)),    BaseHandler },
        { std::type_index(typeid(CommandExecutionException)),   BaseHandler }
    };

    void ErrorHandler::Handle(const BaseException& e, const std::string& context)
    {
        auto it = handlers_.find(typeid(e));
        if (it != handlers_.end())
        {
            it->second(e);
        }
        else
        {
            std::cerr << e.what() << '\n';
        }
    }
}