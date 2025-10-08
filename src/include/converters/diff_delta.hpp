#ifndef SRC_INCLUDE_CONVERTERS_DIFF_DELTA_HPP_
#define SRC_INCLUDE_CONVERTERS_DIFF_DELTA_HPP_

#include <git2/diff.h>

#include <converters/base.hpp>
#include <converters/diff_file.hpp>
#include <gitxx/diff_delta.hpp>

namespace gitxx::internal {

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

};  // namespace gitxx::internal

#endif  // SRC_INCLUDE_CONVERTERS_DIFF_DELTA_HPP_
