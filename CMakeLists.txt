cmake_minimum_required(VERSION 3.28)

# see if we can use ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message("Using ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
else()
  message("ccache not found")
endif()

project(libgit
  VERSION
    0.0.1
  DESCRIPTION
    "c++ wrapper for libgit2"
  HOMEPAGE_URL
    "https://github.com/le1iel/libgit"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/options.cmake)

add_library(libgit SHARED)

set_target_properties(
  libgit
  PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(
  libgit
  PRIVATE
    src/include
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_options(libgit PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_subdirectory(src)
add_subdirectory(include)

include(cmake/deps.cmake)

target_link_libraries(libgit PUBLIC libgit2::libgit2package)

set_property(TARGET libgit PROPERTY CXX_STANDARD 23)

if(LIBGIT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

