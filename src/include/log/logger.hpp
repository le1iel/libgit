#ifndef SRC_INCLUDE_LOG_LOGGING_H
#define SRC_INCLUDE_LOG_LOGGING_H

// #include <array>
// #include <memory>
#include <string_view>

namespace libgit {

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

 private:
  // /// @brief writes
  // void writeToBuffer(std::string_view message);

  // /// @brief buffer for logs
  // std::array<char, 256> m_buffer{};

  // std::size_t m_pos{0U};

  // int m_fd{-1};
};

}  // namespace log

}  // namespace libgit

#endif  // SRC_INCLUDE_LOG_LOGGING_H
