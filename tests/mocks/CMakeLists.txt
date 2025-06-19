# Create object library for all mocks
add_library(${target_test}-mocks OBJECT)
set_target_properties(${target_test}-mocks PROPERTIES LINKER_LANGUAGE CXX)

# Link GMock and GTest
target_link_libraries(${target_test}-mocks
    PUBLIC
        GTest::gmock
        GTest::gtest
        libgit2::libgit2package
)

# Include directories
target_include_directories(${target_test}-mocks
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

# Set C++ standard
target_compile_features(${target_test}-mocks PUBLIC cxx_std_17)

add_subdirectory(libgit2)
