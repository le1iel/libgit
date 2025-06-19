include(GoogleTest)

set(target_test ${PROJECT_NAME}-test)

add_custom_target(${target_test})
set(TEST_GIT_HOME "${CMAKE_CURRENT_BINARY_DIR}/git_home")

include(cmake/create_test.cmake)
include(cmake/create_git_config.cmake)

write_git_config(LOCATION "${TEST_GIT_HOME}")

add_subdirectory(mocks)
add_subdirectory(common)

add_subdirectory(unit)
add_subdirectory(binary)


