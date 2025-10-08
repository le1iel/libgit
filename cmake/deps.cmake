include(FetchContent)

find_package(libgit2 QUIET)
if (libgit2_FOUND)
    # so this is a bit of gymnastics but libgit2 doesn't export a target
    # for the static library, however the brew package does actually have it
    add_library(libgit2::libgit2_static STATIC IMPORTED)

    # get the location of the shared library
    get_target_property(LIBGIT2_SHARED_LOCATION libgit2::libgit2package LOCATION)

    # get the path where the shared library is
    cmake_path(GET LIBGIT2_SHARED_LOCATION PARENT_PATH LIBGIT2_LIB_PATH)

    # the static library is next to the shared one
    set(LIBGIT2_STATIC_LOCATION "${LIBGIT2_LIB_PATH}/libgit2.a")

    # set the properties on the new target
    set_target_properties(libgit2::libgit2_static PROPERTIES
        IMPORTED_LOCATION "${LIBGIT2_STATIC_LOCATION}"
        INTERFACE_INCLUDE_DIRECTORIES "$<TARGET_PROPERTY:libgit2::libgit2package,INTERFACE_INCLUDE_DIRECTORIES>"
        INTERFACE_LINK_LIBRARIES "$<TARGET_PROPERTY:libgit2::libgit2package,INTERFACE_LINK_LIBRARIES>"
    )

else()

    function (cache var value type)
        set(${var} ${value} CACHE ${type} "" FORCE)
    endfunction()

    FetchContent_Declare(libgit2
        GIT_REPOSITORY
            "https://github.com/libgit2/libgit2.git"
        GIT_TAG
            "v1.9.1"
        EXCLUDE_FROM_ALL
    )

    cache(BUILD_SHARED_LIBS OFF BOOL)
    cache(BUILD_TESTS OFF BOOL)
    cache(BUILD_CLI OFF BOOL)
    cache(BUILD_FUZZER OFF BOOL)

    FetchContent_MakeAvailable(libgit2)

    target_include_directories(libgit2package
        INTERFACE
            $<BUILD_INTERFACE:${libgit2_SOURCE_DIR}/include>
    )

    add_library(libgit2::libgit2_static ALIAS libgit2package)

endif()

if(GITXX_BUILD_TESTING)
  find_package(GTest)
  if(NOT GTest_FOUND)
    FetchContent_Declare(
        GTest
      GIT_REPOSITORY
        "https://github.com/google/googletest.git"
      GIT_TAG
        "v1.16.0"
      EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(GTest)
  endif()
endif()
