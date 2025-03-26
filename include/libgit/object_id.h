#ifndef INCLUDE_LIBGIT_OBJECT_ID_H_
#define INCLUDE_LIBGIT_OBJECT_ID_H_

#include <array>
#include <string_view>

namespace git {

class ObjectId {
  static std::optional<ObjectId> FromString(std::string_view id) noexcept;

  std::string_view id() const noexcept;

  std::string_view shortId(std::size_t length) const noexcept;

  int compare(std::string_view other) const noexcept;

  int compare(ObjectId other) const noexcept;

private:
  ObjectId(std::string_view id) noexcept;

  std::array<std::uint8_t, 20> m_id{};
};

} // namespace git
#endif  // INCLUDE_LIBGIT_OBJECT_ID_H_
