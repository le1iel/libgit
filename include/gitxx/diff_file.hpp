#ifndef INCLUDE_GITXX_DIFF_FILE_HPP_
#define INCLUDE_GITXX_DIFF_FILE_HPP_

#include <cstdint>
#include <filesystem>
#include <flagfield.hpp>
#include <gitxx/diff_flag.hpp>
#include <gitxx/object_id.hpp>

namespace gitxx {

/// @brief Description of one side of a delta.
struct DiffFile {
  /// @brief Id.
  ObjectId old_id;
  /// @brief Path to the entry relative to the working directory of the
  ///        repository.
  std::filesystem::path path;
  /// @brief The size of the entry in bytes.
  std::uint64_t size;
  /// @brief Flags for the delta object and the file objects on each side.
  FlagField<DiffFlag> flags;
  /// @brief Roughly, the stat() st_mode value for the item.
  std::uint32_t mode;
};

}  // namespace gitxx

#endif  // INCLUDE_GITXX_DIFF_FILE_HPP_
