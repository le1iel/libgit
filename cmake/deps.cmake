include(FetchContent)

find_package(libgit2)

add_library(libgit2::libgit2_static STATIC IMPORTED)

get_target_property(LIBGIT2_SHARED_LOCATION libgit2::libgit2package LOCATION)
cmake_path(GET LIBGIT2_SHARED_LOCATION PARENT_PATH LIBGIT2_LIB_PATH)

get_target_property(LIBGIT2_INCLUDE_DIRS libgit2::libgit2package INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(LIBGIT2_LINK_LIBS libgit2::libgit2package INTERFACE_LINK_LIBRARIES)

set_target_properties(libgit2::libgit2_static PROPERTIES
    IMPORTED_LOCATION "${LIBGIT2_LIB_PATH}/libgit2.a"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBGIT2_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${LIBGIT2_LINK_LIBS}"
)

if(NOT libgit2_FOUND)

  function (cache var value type)
    set(${var} ${value} CACHE ${type} "" FORCE)
  endfunction()

  FetchContent_Declare(
      libgit2
    GIT_REPOSITORY
      "https://github.com/libgit2/libgit2.git"
    GIT_TAG
      "v1.9.0"
    EXCLUDE_FROM_ALL
  )

  cache(BUILD_SHARED_LIBS OFF BOOL)
  cache(BUILD_TESTS OFF BOOL)
  cache(BUILD_CLI OFF BOOL)

  FetchContent_MakeAvailable(libgit2)
endif()

if(LIBGIT_BUILD_TESTING)
  find_package(GTest)
  if( NOT GTest_FOUND)
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
