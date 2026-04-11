cmake_minimum_required(VERSION 3.28)
project(gitxx_fc_consumer CXX)

include(FetchContent)

FetchContent_Declare(gitxx
    GIT_REPOSITORY "https://github.com/le1iel/libgit.git"
    GIT_TAG        "develop"
)
FetchContent_MakeAvailable(gitxx)

add_executable(fc_example main.cpp)
target_link_libraries(fc_example PRIVATE gitxx::gitxx)
target_compile_features(fc_example PRIVATE cxx_std_23)
