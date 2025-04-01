#include <libgit/diff_delta.h>
#include <git2/diff.h>
#include <iostream>
namespace git {

namespace detail {

DiffFile DiffFileFromGit2(const git_diff_file &file) {
    DiffFile result;
    result.path = std::string(file.path);
    result.size = file.size;
    result.mode = file.mode;
    return result;
}

std::optional<DiffDelta> DiffDeltaFromGit2(const git_diff_delta *delta) {

    if (delta == nullptr) {
        return std::nullopt;
    }

    DiffDelta result;
    result.flags = delta->flags;
    result.similarity = delta->similarity;
    result.nfiles = delta->nfiles;
    result.old_file = DiffFileFromGit2(delta->old_file);
    result.new_file = DiffFileFromGit2(delta->new_file);

    return result;
}

}

} // namespace git