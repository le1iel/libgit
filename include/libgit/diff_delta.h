#ifndef INCLUDE_LIBGIT_DIFF_DELTA_H_
#define INCLUDE_LIBGIT_DIFF_DELTA_H_

#include <cstdint>
#include "diff_file.h"

namespace git {

/// @brief The status of a diff delta.
enum class DiffDeltaStatus {
  Unmodified = 1,
  Added = 3,
  Deleted = 5,
  Modified = 7,
  Renamed = 9,
  Copied = 11,
  Ignored = 13,
  Untracked = 15,
  TypeChange = 17,
  Unreadable = 19,
  Conflicted = 21,
};

struct DiffDelta {
  DiffDeltaStatus status;
  std::uint32_t flags;
  /// @brief How similar the file is.
  std::uint16_t similarity;
  /// @brief The number of files in the delta.
  std::uint16_t nfiles;
  /// @brief The old file.
  DiffFile old_file;
  /// @brief The new file.
  DiffFile new_file;
};

} // namespace git

#endif // INCLUDE_LIBGIT_DIFF_DELTA_H_
