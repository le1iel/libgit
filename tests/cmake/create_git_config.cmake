function(write_git_config)
  cmake_parse_arguments(
    ARG
    ""
    "LOCATION"
    ""
    ${ARGN}
  )

  configure_file(
      "${CMAKE_CURRENT_LIST_DIR}/cmake/in/.gitconfig"
      "${ARG_LOCATION}/.gitconfig"
      COPYONLY
  )
endfunction()
