#ifndef INCLUDE_GITXX_DIFF_FLAG_HPP_
#define INCLUDE_GITXX_DIFF_FLAG_HPP_

#include <cstdint>

namespace gitxx {

/// @brief Flags for the delta object and the file objects on each side.
enum class DiffFlag: std::uint8_t {
  /// @brief File(s) treated as binary data.
  Binary = 0,
  /// @brief File(s) treated as text data.
  NotBinary = 1,
  /// @brief id value is known correct.
  ValidId = 2,
  /// @brief File exists at this side of the delta.
  Exists = 3,
  /// @brief File size value is known correct.
  Valid_size = 4,
};

}  // namespace gitxx

#endif  // INCLUDE_GITXX_DIFF_FLAG_HPP_
