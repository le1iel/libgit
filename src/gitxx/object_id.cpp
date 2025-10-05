#include <algorithm>
#include <gitxx/object_id.hpp>
#include <ostream>

namespace gitxx {

ObjectId::ObjectId(ObjectId::TagView tag) noexcept {

  std::ranges::copy(tag, m_id.begin());
}

std::string_view ObjectId::id() const noexcept {
  return std::string_view{reinterpret_cast<const char*>(m_id.begin()),
                          m_id.size()};
}

std::ostream& operator<<(std::ostream& stream, const ObjectId& objectId) noexcept {
  return stream << objectId.id();
}

bool operator==(const ObjectId& lhs, const ObjectId& rhs) noexcept {
  return lhs.m_id == rhs.m_id;
}

}  // namespace gitxx
