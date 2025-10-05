# Get include directories from libgit2 target
get_target_property(LIBGIT2_INCLUDE_DIRS libgit2::libgit2_static INTERFACE_INCLUDE_DIRECTORIES)

# Add mock sources to the parent mocks object library
target_sources(${target_test}-mocks
    PRIVATE
        mock_repository.h
)

# Add libgit2 include directories to mocks
target_include_directories(${target_test}-mocks
    PRIVATE
        ${LIBGIT2_INCLUDE_DIRS}
)
