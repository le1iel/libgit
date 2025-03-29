set(target_test_binary ${target_test}-binary)
add_custom_target(${target_test_binary})
add_dependencies(${target_test} ${target_test_binary})

add_subdirectory(bt_repository)
