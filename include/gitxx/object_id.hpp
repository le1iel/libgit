#ifndef INCLUDE_GITXX_OBJECT_ID_HPP_
#define INCLUDE_GITXX_OBJECT_ID_HPP_

#include <array>
#include <cstdint>
#include <span>
#include <string_view>

namespace gitxx {

class ObjectId {
 public:
  ObjectId() = default;

  ObjectId(std::span<const std::uint8_t, 20> id) noexcept;

  std::string_view id() const noexcept;

  /// @brief Spaceship operator.
  friend bool operator==(const ObjectId& lhs, const ObjectId& rhs) noexcept;

  friend std::ostream& operator<<(std::ostream& stream,
                                  const ObjectId& id) noexcept;

 private:
  ObjectId(std::string_view id) noexcept;

  std::array<std::uint8_t, 20> m_id{};
};

}  // namespace gitxx
#endif  // INCLUDE_GITXX_OBJECT_ID_HPP_
