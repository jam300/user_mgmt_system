#include "commandresult/OutputPrinter.h"
#include "utils/Symbols.h"
#include <locale>

using namespace  Symbols;

namespace CommandResult
{
    void OutputPrinter::PrintCommandSuccess(const std::string& commandLine)
    {
        std::cout << SYMBOL_SUCCESS << " " << commandLine << '\n';
    }
    void OutputPrinter::PrintCommandResult(const std::string& commandLine)
    {
        std::cout << "    "<< commandLine << '\n';
    }

    void OutputPrinter::PrintCommandFailure(const std::string& commandLine, const std::string& failureReason)
    {
        std::cout << SYMBOL_FAILURE << " " << commandLine << " (Failed: " << failureReason << ")\n";
    }

    void OutputPrinter::PrintTaskStart(const std::string& taskName)
    {
        std::cout << "[Processing task: " << taskName << "]\n";
    }

    void OutputPrinter::PrintTaskFailure(const std::string& taskName)
    {
        std::cout << "[Task " << taskName << " stopped due to failure]\n";
        std::cout << std::endl;
    }

    void OutputPrinter::PrintTaskSuccess(const std::string& taskName)
    {
        std::cout << "[Task " << taskName << " completed successfully]\n";
        std::cout << std::endl;
    }
}
