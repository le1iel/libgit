add_library(gitxx-test-common INTERFACE)

target_include_directories(gitxx-test-common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# git_commands2.hpp calls libgit2 directly, so expose its headers to consumers.
target_link_libraries(gitxx-test-common INTERFACE
    libgit2::libgit2_static
)
