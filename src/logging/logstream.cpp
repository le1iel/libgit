#include <log/logstream.h>
#include <unistd.h>

namespace libgit {

namespace log {

LogStream::LogStream(): m_fd(STDOUT_FILENO) {}

void LogStream::writeToBuffer(const std::string& message) {
    std::copy(message.begin(), message.end(), m_buffer.begin());
    write(m_fd, m_buffer.data(), message.size());
}

} // namespace log  

} // namespace libgit