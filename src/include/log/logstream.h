#ifndef SRC_INCLUDE_LOG_LOGSTREAM_H
#define SRC_INCLUDE_LOG_LOGSTREAM_H

#include <string>
#include <array>

namespace libgit {

namespace log {

class LogStream {
 public:
  LogStream();
  ~LogStream();

  LogStream& operator<<(const std::string& message);
  LogStream& operator<<(const char* message);
  LogStream& operator<<(const int& message);
  LogStream& operator<<(const long& message);
  LogStream& operator<<(const float& message);
  LogStream& operator<<(const double& message);
  
private:
    /// @brief Write to the buffer
    void writeToBuffer(const std::string& message);

    /// @brief File descriptor to write to
    int m_fd;

    /// @brief Buffer to store the log message
    std::array<char, 256> m_buffer {};
};

} // namespace log

} // namespace libgit

#endif // SRC_INCLUDE_LOG_LOGSTREAM_H
