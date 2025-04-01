#include <libgit/diff_delta.h>
#include <git2/diff.h>

namespace git {

namespace detail{

DiffFile DiffFileFromGit2(const git_diff_file *file);

DiffDelta DiffDeltaFromGit2(const git_diff_delta *delta);

} // namespace detail

} // namespace git