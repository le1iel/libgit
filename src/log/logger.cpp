#include <log/logger.hpp>
#include <unistd.h>
#include <iostream>

namespace libgit {

namespace log {

void
Log::info(const std::string_view message)
{
    std::cout << "[INFO] " << message << "\n";
}

void
Log::error(const std::string_view message)
{
    std::cout << "[ERROR] " << message << "\n";
}

void
Log::debug(const std::string_view message)
{
    std::cout << "[DEBUG] " << message << "\n";
}

}  // namespace log

}  // namespace libgit
