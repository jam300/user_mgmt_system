#include "errorhandling/exceptions/AllExceptions.h"
#include <functional>
#include <unordered_map>
#include <typeindex>
#include<string>

namespace ErrorHandling::Exceptions
{
    class ErrorHandler
    {
        public:
            using HandlerFunc = std::function<void(const BaseException&)>;

            static void Handle(const BaseException& e, const std::string& context = "");

        private:
            static const std::unordered_map<std::type_index, HandlerFunc> handlers_;
    };
}

