include(GoogleTest)

set(target_test ${PROJECT_NAME}-test)

add_custom_target(${target_test})

include(cmake/create_test.cmake)

# TODO ISSUE #2
# add_subdirectory(mocks)
add_subdirectory(common)

# add_subdirectory(unit)
add_subdirectory(binary)

