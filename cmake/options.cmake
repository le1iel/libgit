option(GITXX_BUILD_STATIC_LIBS "Build the static library" ON)

option(GITXX_BUILD_SHARED_LIBS "Build the shared library" OFF)

option(BUILD_TESTING "Build test programs" ON)

option(GITXX_BUILD_TESTING "Build test programs" ${PROJECT_IS_TOP_LEVEL})

option(GITXX_SANITIZERS "Build tests with address and undefined-behaviour sanitizers" ${PROJECT_IS_TOP_LEVEL})

option(GITXX_INTEGRATION_TESTS "Build and register integration tests" OFF)
