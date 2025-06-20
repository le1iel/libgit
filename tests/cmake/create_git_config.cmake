set(TEST_GIT_HOME "${CMAKE_CURRENT_BINARY_DIR}/git_home")

configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/in/.gitconfig"
  "${TEST_GIT_HOME}/.gitconfig"
  COPYONLY
)
