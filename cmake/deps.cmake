
include(FetchContent)

FetchContent_Declare(
        libgit2
    GIT_REPOSITORY
        "https://github.com/libgit2/libgit2.git"
    GIT_TAG
        "v1.9.0"
    CMAKE_ARGS
        "-DBUILD_TESTS=OFF -DBUILD_CLI=OFF -DBUILD_SHARED_LIBS=OFF"
    )
FetchContent_MakeAvailable(libgit2
)

find_package(libgit2)

if(BUILD_TESTS)
FetchContent_Declare(
        GTest
    GIT_REPOSITORY
        "https://github.com/google/googletest.git"
    GIT_TAG
        "v1.16.0"
    )
    FetchContent_MakeAvailable(GTest)
    find_package(GTest)
endif()
