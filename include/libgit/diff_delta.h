#ifndef INCLUDE_LIBGIT_DIFF_DELTA_H_
#define INCLUDE_LIBGIT_DIFF_DELTA_H_

#include <cstdint>

namespace git {

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
  std::uint16_t similarity;
  std::uint16_t nfiles;
};

} // namespace git

#endif // INCLUDE_LIBGIT_DIFF_DELTA_H_
