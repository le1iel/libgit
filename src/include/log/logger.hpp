#ifndef SRC_INCLUDE_LOG_LOGGING_H
#define SRC_INCLUDE_LOG_LOGGING_H

#include <string_view>

namespace gitxx {

namespace log {

/// @brief Log levels
enum class LogLevel { Error, Warning, Info, Debug };

class Log {
 public:
  static void error(const std::string_view message);

  static void warning(const std::string_view message);

  static void info(const std::string_view message);

  static void debug(const std::string_view message);

  ~Log() = default;
  Log();

};

}  // namespace log

}  // namespace gitxx

#endif  // SRC_INCLUDE_LOG_LOGGING_H
