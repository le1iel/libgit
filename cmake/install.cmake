include(CMakePackageConfigHelpers)

include(GNUInstallDirs)

# Configure the config file
configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/in/gitxx_config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/gitxx_config.cmake"
  INSTALL_DESTINATION
    "lib/cmake/gitxx"
)

install(
  TARGETS gitxx-lib
  EXPORT gitxx-targets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.hpp"
)

install(
    EXPORT gitxx-targets
    FILE gitxx_targets.cmake
    NAMESPACE gitxx::
    DESTINATION lib/cmake/gitxx
)
