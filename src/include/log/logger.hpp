#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <source_location>  // C++20 feature

// Instead of stream macros, use inline references to streams
namespace logging {
    // Base stream references
    inline std::ostream& error_stream = std::cerr;
    inline std::ostream& out_stream = std::cout;
    
    // Error logger class with cout-like syntax
    class ErrorLogger {
    private:
        // Store a reference to the output stream
        std::ostream& stream_;
        bool prefix_added_ = false;
        
    public:
        // Constructor that prepares the stream
        constexpr explicit ErrorLogger(std::ostream& stream = error_stream) 
            : stream_(stream) {}
        
        // Destructor to ensure flushing
        ~ErrorLogger() {
            stream_ << std::endl;
        }

        ErrorLogger(ErrorLogger& other) = delete;
        ErrorLogger(const ErrorLogger&& other) = delete;
        
        // Template method to handle any type of output
        template<typename T>
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
            const std::source_location& location = std::source_location::current()
        ) :  location_(location) {
            *this << "[" << location_.file_name() << ":" << location_.line() << "] ";
        }
    };
    
    // Factory functions that return temporary loggers
    constexpr auto ERROR() { return ErrorLogger(); }
    
    // C++20 version with source information
    constexpr auto ERROR_LOC(
        const std::source_location& location = std::source_location::current()
    ) {
        return SourceErrorLogger(location);
    }
}

#endif // LOGGING_HPP
