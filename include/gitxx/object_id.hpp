#ifndef INCLUDE_GITXX_OBJECT_ID_HPP_
#define INCLUDE_GITXX_OBJECT_ID_HPP_

#include <array>
#include <cstdint>
#include <span>
#include <string_view>

#define TAG_LENGTH 20

namespace gitxx {

/// @brief Unique identity of any object (commit, tree, blob, tag).
class ObjectId {
    static constexpr std::uint8_t TagLength { 20 };

 public:
    using Tag = std::array<std::byte, TagLength>;
    using TagView = std::span<const std::byte, TagLength>;

  /// @brief Default constructable.
  ObjectId() = default;

  /// @brief Constructor from hexadecimal.
  explicit ObjectId(TagView tag) noexcept;

  /// @brief Id of the object.
  [[nodiscard]] std::string_view id() const noexcept;

  /// @brief Equality operator.
  friend bool operator==(const ObjectId& lhs, const ObjectId& rhs) noexcept;

  /// @brief Writes the ObjectId to an output stream as a hexadecimal string.
  friend std::ostream& operator<<(std::ostream& stream,
                                  const ObjectId& objectId) noexcept;

 private:
  /// @brief Constructor from string_view.
  explicit ObjectId(std::string_view tag) noexcept;

  /// @brief Internal representaion of the id.
  Tag m_id{};

};

  /// @brief Equality operator.
[[nodiscard]] bool operator==(const ObjectId& lhs,
                              const ObjectId& rhs) noexcept;

}  // namespace gitxx
#endif  // INCLUDE_GITXX_OBJECT_ID_HPP_
