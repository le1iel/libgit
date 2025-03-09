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

    if(NOT ARG_NAME)
        message(FATAL_ERROR "You must provide a name")
    endif()

    
    # TODO coverage
    # set_source_files_properties(
    #         ${ARG_SRC}
    #     PROPERTIES
    #         COMPILE_FLAGS "-g -fprofile-arcs -ftest-coverage"
    # )

    # set_source_files_properties(
    #         ${ARG_SRC}
    #     PROPERTIES
    #         LINK_FLAGS "-fprofile-arcs -ftest-coverage -lgcov -lcoverage")

    add_executable(
        ${target}
        "${ARG_SRC}"
    )


    target_link_libraries(
        ${target}
        PUBLIC
        GTest::gtest_main
        libgit
        libgit2::libgit2package
    )

    set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)

    set_target_properties(${target} PROPERTIES OUTPUT_NAME "${ARG_NAME}")

    add_test(
        NAME ${ARG_NAME}
        COMMAND ${target}
    )
endfunction()
