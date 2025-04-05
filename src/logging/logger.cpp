#include <log/logger.h>
#include <unistd.h>

namespace libgit {

namespace log {

Log::Log(int fd) : m_fd(fd) {}

Log& Log::getLogger() {
  if (not m_instance) std::unique_ptr<Log>(new Log(STDOUT_FILENO));
  return *m_instance.get();
}

void
Log::writeToBuffer(const std::string_view message)
{
    std::copy(message.begin(), message.end(), std::next(m_buffer.begin(), m_pos));
}

void
Log::error(const std::string_view message)
{
    Log::getLogger();
}

}  // namespace log

}  // namespace libgit
