
include(FetchContent)

find_package(libgit2)
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
        # CMAKE_ARGS
        #     "-DBUILD_TESTS OFF -DBUILD_CLI OFF"
        EXCLUDE_FROM_ALL
        )

    cache(BUILD_SHARED_LIBS OFF BOOL)
    cache(BUILD_TESTS OFF BOOL)
    cache(BUILD_CLI OFF BOOL)

    FetchContent_MakeAvailable(libgit2)
    add_library(libgit2::libgit2package ALIAS libgit2)
endif()

if(LIBGIT_BUILD_TESTS)
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
