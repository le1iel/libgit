#ifndef SRC_INCLUDE_LOG_LOGGING_H
#define SRC_INCLUDE_LOG_LOGGING_H

#include <memory>
#include <string_view>
#include <array>

namespace libgit {

namespace log {

enum class LogLevel { Error, Warning, Info, Debug };

class Log {
 public:
  static Log& getLogger();
  static void error(const std::string_view message);
  static void warning(const std::string_view message);
  // static void info(const std::string& message);
  // static void debug(const std::string& message);

 private:
  void writeToBuffer(std::string_view message);

  Log(int fd);

  ~Log() = default;

  std::array<char, 256> m_buffer {};

  std::size_t m_pos;

  static std::unique_ptr<Log> m_instance;
  int m_fd;
};

}  // namespace log

}  // namespace libgit

#endif  // SRC_INCLUDE_LOG_LOGGING_H
