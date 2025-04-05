#ifndef INCLUDE_LIBGIT_DIFF_DELTA_H_
#define INCLUDE_LIBGIT_DIFF_DELTA_H_

#include <cstdint>
#include "diff_file.h"

namespace git {

/// @brief The status of a diff delta.
enum class DiffDeltaStatus {
  Unmodified = 0,
  Added = 1,
  Deleted = 2,
  Modified = 3,
  Renamed = 4,
  Copied = 5,
  Ignored = 6,
  Untracked = 7,
  TypeChange = 8,
  Unreadable = 9,
  Conflicted = 10,
};

struct DiffDelta {
  DiffDeltaStatus status;
  std::uint32_t flags;
  ///@brief How similar the file is.
  std::uint16_t similarity;
  ///@brief The number of files in the delta.
  std::uint16_t nfiles;
  ///@brief The old file.
  DiffFile old_file;
  ///@brief The new file.
  DiffFile new_file;
};

} // namespace git

#endif // INCLUDE_LIBGIT_DIFF_DELTA_H_
