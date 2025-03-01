enable_testing()
message("ASD")

function(create_test)

    cmake_parse_arguments(
        ARG
        ""
        "NAME;SRC"
        ""
        ${ARGN}
    )


    set(target ${target_test_unit}-${ARG_NAME})
    add_custom_target(target)
    add_dependencies(${target_test_unit} ${target})

    message("${target}")

    if(NOT ARG_NAME)
        message(FATAL_ERROR "You must provide a name")
    endif()

    add_executable(
        ${target}
        "${ARG_SRC}"
    )

    target_link_libraries(
        ${target}
        GTest::gtest_main
        ${PROJECT_NAME}
    )

    add_test(
        NAME ${ARG_NAME}
        COMMAND ${target}
    )
endfunction()
