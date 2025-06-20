# ---- Create Test ---
#
function(create_test)
  cmake_parse_arguments(
    ARG
      ""
      "NAME;SRC;DEPENDENCY"
      "LINKS;INCLUDES"
      ${ARGN}
  )

  set(target ${ARG_DEPENDENCY}-${ARG_NAME})

  if(NOT ARG_NAME)
    message(FATAL_ERROR "You must provide a name")
  endif()

  add_executable(
    ${target}
    "${ARG_SRC}"
  )
  add_dependencies(${ARG_DEPENDENCY} ${target})

  target_link_libraries(
        ${target}
    PUBLIC
        "${ARG_LINKS}"
        GTest::gtest_main
        gitxx-test-common
  )

  target_include_directories(
    ${target}
    PUBLIC
    "${ARG_INCLUDES}"
  )

  set_property(TARGET ${target} PROPERTY CXX_STANDARD 23)

  set_target_properties(${target} PROPERTIES OUTPUT_NAME "${ARG_NAME}")
  target_compile_options(${target} PRIVATE -fsanitize=address)
  target_link_options(${target} PRIVATE -fsanitize=address)

  target_compile_definitions(${target} PRIVATE TEST_GIT_HOME="${TEST_GIT_HOME}")


  add_test(
    NAME ${ARG_NAME}
    COMMAND ${target}
  )

  if(GITXX_COVERAGE)
    target_compile_options(${target} PRIVATE -g -O0 -fprofile-instr-generate -fcoverage-mapping)
    target_link_libraries(${target} PRIVATE -fprofile-instr-generate)

    set_tests_properties(${ARG_NAME} PROPERTIES
        ENVIRONMENT "LLVM_PROFILE_FILE=${TEST_COVERAGE_DIR}/coverage_%p_%m.profraw"
    )
  endif()

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
      "${target_test}-binary"
    LINKS
      "${ARG_LINKS}"
      gitxx-lib
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
            GTest::gmock_main
            ${target_test}-mocks
    )
endfunction()
