cmake_minimum_required(VERSION 3.28)

include(cmake/ccache.cmake)

project(gitxx
  VERSION
    0.0.1
  DESCRIPTION
    "c++ wrapper for libgit2"
  HOMEPAGE_URL
    "https://github.com/le1iel/libgit"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/options.cmake)

add_library(gitxx-lib SHARED)

include(cmake/install.cmake)

set_target_properties(
    gitxx-lib
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(
    gitxx-lib
  PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
  PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>"
)

target_compile_options(gitxx-lib PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_subdirectory(src)
add_subdirectory(include)

include(cmake/deps.cmake)

target_link_libraries(gitxx-lib
  PUBLIC
    libgit2::libgit2_static
)

set_property(TARGET gitxx-lib PROPERTY CXX_STANDARD 23)

if(GITXX_BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
