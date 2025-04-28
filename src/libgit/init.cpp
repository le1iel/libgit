#include <git2.h>

#include <libgit/init.hpp>

void init_libgit() {
    git_libgit2_init();
}

void deinit_libgit() {
    git_libgit2_shutdown();
}
