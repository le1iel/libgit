#include <algorithm>
#include <libgit/object_id.hpp>
#include <memory>
#include <optional>
namespace git {

ObjectId::ObjectId(std::span<const std::uint8_t, 20> id) noexcept {
  std::copy(id.begin(), id.end(), m_id.begin());
}

std::string_view ObjectId::id() const noexcept {
  return std::string_view{reinterpret_cast<const char*>(m_id.begin()),
                          m_id.size()};
}

std::ostream& operator<<(std::ostream& stream, const ObjectId& id) noexcept {
  return stream << id.id();
}

bool operator==(const ObjectId& lhs, const ObjectId& rhs) noexcept {
  return lhs.m_id == rhs.m_id;
}

}  // namespace git
