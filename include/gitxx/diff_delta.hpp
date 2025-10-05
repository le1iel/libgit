#ifndef INCLUDE_GITXX_DIFF_DELTA_HPP_
#define INCLUDE_GITXX_DIFF_DELTA_HPP_

#include <cstdint>
#include <flagfield.hpp>
#include <gitxx/diff_file.hpp>
#include <gitxx/diff_flag.hpp>

namespace gitxx {

/// @brief The status of a diff delta.
enum class DiffDeltaStatus : std::uint8_t {
  /// @brief No changes.
  Unmodified = 1,
  /// @brief Entry does not exist in old version
  Added = 3,
  /// @brief Entry does not exist in new version.
  Deleted = 5,
  /// @brief Entry content changed between old and new.
  Modified = 7,
  /// @brief Entry was renamed between old and new.
  Renamed = 9,
  /// @brief Entry was copied from another old entry.
  Copied = 11,
  /// @brief Entry is ignored item in workdir.
  Ignored = 13,
  /// @brief Entry is untracked item in workdir.
  Untracked = 15,
  /// @brief Type of entry changed between old and new.
  TypeChange = 17,
  /// @brief Entry is unreadable.
  Unreadable = 19,
  /// @brief Entry in the index is conflicted.
  Conflicted = 21,
};

/// @brief Description of changes to one entry.
struct DiffDelta {
  /// @brief Status of the delta.
  DiffDeltaStatus status{};
  /// @brief Flags for the delta object and the file objects on each side.
  FlagField<DiffFlag> flags;
  /// @brief How similar the file is 0-100.
  std::uint16_t similarity{};
  /// @brief The number of files in the delta.
  std::uint16_t nfiles{};
  /// @brief The old file.
  DiffFile old_file;
  /// @brief The new file.
  DiffFile new_file;
};

}  // namespace gitxx

#endif  // INCLUDE_GITXX_DIFF_DELTA_HPP_
