#include <libgit/init.h>
#include <git2.h>

void init_libgit() {
    git_libgit2_init();
}

void deinit_libgit() {
    git_libgit2_shutdown();
}