#ifndef INCLUDE_GITXX_OBJECT_ID_HPP_
#define INCLUDE_GITXX_OBJECT_ID_HPP_

#include <array>
#include <cstdint>
#include <span>
#include <string_view>

namespace gitxx {

/// @brief Unique identity of any object (commit, tree, blob, tag).
class ObjectId {
 public:
  /// @brief Default constructable.
  ObjectId() = default;

  /// @brief Constructor from hexadecimal.
  ObjectId(std::span<const std::uint8_t, 20> id) noexcept;

  /// @brief Copy constructable.
  ObjectId(const ObjectId&) = default;

  /// @brief Copy assignable.
  ObjectId &operator=(const ObjectId&) = default;

  /// @brief Move constructable.
  ObjectId(ObjectId&&) = default;

  /// @brief Move assignable.
  ObjectId &operator=(ObjectId&&) = default;

  /// @brief Id of the object.
  [[nodiscard]] std::string_view id() const noexcept;

  /// @brief Equality operator.
  friend bool operator==(const ObjectId& lhs, const ObjectId& rhs) noexcept;

  /// @brief Writes the ObjectId to an output stream as a hexadecimal string.
  friend std::ostream& operator<<(std::ostream& stream,
                                  const ObjectId& id) noexcept;

 private:
  /// @brief Constructor from string_view.
  ObjectId(std::string_view id) noexcept;

  /// @brief Internal representaion of the id.
  std::array<std::uint8_t, 20> m_id{};
};

  /// @brief Equality operator.
[[nodiscard]] bool operator==(const ObjectId& lhs,
                              const ObjectId& rhs) noexcept;

}  // namespace gitxx
#endif  // INCLUDE_GITXX_OBJECT_ID_HPP_
