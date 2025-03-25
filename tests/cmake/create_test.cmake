function(create_test)
    cmake_parse_arguments(
        ARG
        ""
        "NAME;SRC;DEPENDENCY"
        "LINKS;INCLUDES"
        ${ARGN}
    )
    message("--${ARG_DEPENDENCY}")

    set(target ${ARG_DEPNDENCY}-${ARG_NAME})
    add_custom_target(target)
    add_dependencies(${ARG_DEPENDENCY} ${target})

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
        "${ARG_LINKS}"
        GTest::gtest_main
        # libgit
        # libgit2::libgit2package
    )

    target_include_directories(
        ${target}
        PUBLIC
        "${ARG_INCLUDES}"
    )

    set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)

    set_target_properties(${target} PROPERTIES OUTPUT_NAME "${ARG_NAME}")

    add_test(
        NAME ${ARG_NAME}
        COMMAND ${target}
    )
endfunction()

function(create_binary_test)
    cmake_parse_arguments(
        ARG
        ""
        "NAME;SRC"
        "LINKS;INCLUDES"
        ${ARGN}
    )

    create_test(
        NAME
            "${ARG_NAME}"
        SRC
            "${ARG_SRC}"
        DEPENDENCY
            ${target_test}-binary
        LINKS
            "${ARG_LINKS}"
            libgit
            libgit2::libgit2package
    )
endfunction()

function(create_unit_test)
    cmake_parse_arguments(
        ARG
        ""
        "NAME;SRC"
        "LINKS;INCLUDES"
        ${ARGN}
    )

    create_test(
        NAME
            "${ARG_NAME}"
        SRC
            "${ARG_SRC}"
        DEPENDENCY
            ${target_test}-unit
        LINKS
            "${ARG_LINKS}"
            ${target_test}-mock
    )
endfunction()
