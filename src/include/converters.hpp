#include <git2/status.h>

#include <cstdlib>
#include <gitxx/diff_delta.hpp>
#include <gitxx/error.hpp>
#include <gitxx/status.hpp>

#include "git2/diff.h"
#include "gitxx/status_options.hpp"

namespace gitxx::internal {

template <typename CppType, typename CType>
struct conversion_traits;

// TODO How to move these to the cpp files
template <>
struct conversion_traits<GitErrc, int> {
  static GitErrc from_c(const int& error) {
    return static_cast<GitErrc>(std::abs(error));
  }

  static int from_cpp(const GitErrc& error) { return static_cast<int>(error); }
};

template <>
struct conversion_traits<git_status_options, gitxx::StatusOptions> {
  static std::optional<gitxx::StatusOptions> from_c(
      const git_status_options* option) {
    // do we ever need this?
    static_cast<void>(option);
    return std::nullopt;
  }

  static git_status_options from_cpp(const gitxx::StatusOptions& options) {
    return {.version = 1U,
            .show = static_cast<git_status_show_t>(options.show),
            .flags = options.flags.value(),
            .pathspec = {nullptr, 0U},
            .baseline = nullptr,
            .rename_threshold = options.rename_threshold};
  }
};

template <>
struct conversion_traits<gitxx::DiffFile, git_diff_file> {
  static gitxx::DiffFile from_c(const git_diff_file& file) {
    return gitxx::DiffFile{
        .old_id{},
        .path{std::string(file.path)},
        .size = file.size,
        .flags = gitxx::FlagField<gitxx::DiffFlag>{0U},
        .mode = file.mode,
    };
  }
};

template <>
struct conversion_traits<gitxx::DiffDelta, git_diff_delta> {
  static std::optional<gitxx::DiffDelta> from_c(const git_diff_delta* delta) {
    if (delta == nullptr) {
      return std::nullopt;
    }

    return gitxx::DiffDelta{
        .status = gitxx::DiffDeltaStatus::Unmodified,
        .flags = gitxx::FlagField<gitxx::DiffFlag>{delta->flags},
        .similarity = delta->similarity,
        .nfiles = delta->nfiles,
        .old_file = conversion_traits<gitxx::DiffFile, git_diff_file>::from_c(
            delta->old_file),
        .new_file = conversion_traits<gitxx::DiffFile, git_diff_file>::from_c(
            delta->new_file),
    };
  }
};

}  // namespace gitxx::internal
