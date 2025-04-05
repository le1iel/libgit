#ifndef SRC_INCLUDE_LOG_LOGGING_H
#define SRC_INCLUDE_LOG_LOGGING_H

#include <string>

namespace libgit {

namespace log {

enum class LogLevel {
  Error,
  Warning,
  Info,
  Debug
};

class Log {
 public:
  Log();

  static void LogError(const std::string& message);
  static void LogWarning(const std::string& message);
  static void LogInfo(const std::string& message);
  static void LogDebug(const std::string& message);
};

} // namespace log

} // namespace libgit

#endif // SRC_INCLUDE_LOG_LOGGING_H