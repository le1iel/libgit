add_custom_target(${target_test}-mock-libgit2)
add_dependencies(${target_test}-mock ${target_test}-mock-libgit2)

target_sources(${target_test}-mock-libgit2
    PRIVATE 
    ${LIBGIT_INCLUDE_DIRS}
        mock_repository.h
)
