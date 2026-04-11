#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <source_location>

namespace logging {
// Base stream references
inline std::ostream& error_stream = std::cerr;
inline std::ostream& out_stream = std::cout;

class ErrorLogger {
 private:
  std::ostream& stream_;
  bool prefix_added_ = false;

 public:
  // Constructor that prepares the stream
  constexpr explicit ErrorLogger(std::ostream& stream = error_stream)
      : stream_(stream) {}

  ErrorLogger(const ErrorLogger&) = delete;
  ErrorLogger(ErrorLogger&&) = delete;
  ErrorLogger& operator=(const ErrorLogger&) = delete;
  ErrorLogger& operator=(ErrorLogger&&) = delete;

  // Destructor to ensure flushing
  ~ErrorLogger() { stream_ << '\n' << std::flush; }

  // Template method to handle any type of output
  template <typename T>
  ErrorLogger& operator<<(const T& value) {
    if (!prefix_added_) {
      stream_ << "[ERROR] ";
      prefix_added_ = true;
    }
    stream_ << value;
    return *this;
  }

  // Handle manipulators like std::endl
  ErrorLogger& operator<<(std::ostream& (*manip)(std::ostream&)) {
    stream_ << manip;
    return *this;
  }
};

// C++20 version with source location
class SourceErrorLogger : public ErrorLogger {
 private:
  std::source_location location_;

 public:
  // Constructor that captures source location
  constexpr explicit SourceErrorLogger(
      const std::source_location& location = std::source_location::current())
      : location_(location) {
    *this << "[" << location_.file_name() << ":" << location_.line() << "] ";
  }
};

constexpr auto ERROR() { return ErrorLogger(); }

constexpr auto ERROR_LOC(
    const std::source_location& location = std::source_location::current()) {
  return SourceErrorLogger(location);
}
}  // namespace logging

#endif  // LOGGING_HPP
