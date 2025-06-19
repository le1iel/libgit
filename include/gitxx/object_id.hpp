#ifndef INCLUDE_LIBGIT_OBJECT_ID_H_
#define INCLUDE_LIBGIT_OBJECT_ID_H_

#include <array>
#include <cstdint>
#include <span>
#include <string_view>

namespace git {

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

}  // namespace git
#endif  // INCLUDE_LIBGIT_OBJECT_ID_H_
