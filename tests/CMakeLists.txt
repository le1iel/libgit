include(GoogleTest)

set(target_test ${PROJECT_NAME}-test)

add_custom_target(${target_test})

if(GITXX_COVERAGE)
    include(cmake/setup_coverage.cmake)
endif()

include(cmake/create_git_config.cmake)
include(cmake/create_test.cmake)

add_subdirectory(mocks)
add_subdirectory(common)

add_subdirectory(unit)
add_subdirectory(binary)
