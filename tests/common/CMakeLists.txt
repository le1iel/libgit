add_library(gitxx-test-common INTERFACE)

target_include_directories(gitxx-test-common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
