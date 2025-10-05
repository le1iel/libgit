#include <git2/global.h>
///

#include <gitxx/init.hpp>

int init_libgit() {
  // ToDo: can this be moved to repo? should be the only entry points?
  // how to shutdown then tho

  return git_libgit2_init();
}

void deinit_libgit() { git_libgit2_shutdown(); }
