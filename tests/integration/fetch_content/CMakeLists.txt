set(fc_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/example")
set(fc_build_dir  "${CMAKE_CURRENT_BINARY_DIR}/example")
set(fc_bin_dir    "${fc_build_dir}/bin")

add_test(
    NAME integration.fetch_content.configure
    COMMAND ${CMAKE_COMMAND}
        -S "${fc_source_dir}"
        -B "${fc_build_dir}"
        -DFETCHCONTENT_SOURCE_DIR_GITXX=${PROJECT_SOURCE_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${fc_bin_dir}
)

add_test(
    NAME integration.fetch_content.build
    COMMAND ${CMAKE_COMMAND} --build "${fc_build_dir}"
)

add_test(
    NAME integration.fetch_content.run
    COMMAND "${fc_bin_dir}/fc_example" "${PROJECT_SOURCE_DIR}"
)

set_tests_properties(integration.fetch_content.configure
    PROPERTIES FIXTURES_SETUP fc_configure
)

set_tests_properties(integration.fetch_content.build
    PROPERTIES
        FIXTURES_REQUIRED fc_configure
        FIXTURES_SETUP    fc_build
)

set_tests_properties(integration.fetch_content.run
    PROPERTIES FIXTURES_REQUIRED fc_build
)
