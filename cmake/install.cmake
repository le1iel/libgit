include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/gitxx-config-version.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY ExactVersion
    # version is lower than 1.0.0 so everything is breaking
)

# Configure the config file
configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/in/gitxx-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/gitxx-config.cmake"
  INSTALL_DESTINATION
    "lib/cmake/gitxx"
)
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/gitxx-config.cmake"
    DESTINATION lib/cmake/gitxx
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
    FILE gitxx-targets.cmake
    NAMESPACE gitxx::
    DESTINATION lib/cmake/gitxx
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/gitxx-config-version.cmake"
    DESTINATION lib/cmake/gitxx
)
