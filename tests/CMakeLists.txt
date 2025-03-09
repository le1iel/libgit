include(GoogleTest)

set(target_test ${PROJECT_NAME}-test)

add_custom_target(${target_test})

include(cmake/create_test.cmake)

add_subdirectory(unit)
# add_subdirectory(common)
