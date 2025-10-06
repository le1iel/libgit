#include <iostream>

#define ERROR_STREAM std::cerr
#define OUT_STREAM std::cout

// Macro that creates a temporary stream for cout-like syntax
#define ERROR() ERROR_STREAM << "[ERROR] "

