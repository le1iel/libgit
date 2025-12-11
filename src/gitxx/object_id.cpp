#include <algorithm>
#include <gitxx/object_id.hpp>
#include <ostream>
#include <string_view>

namespace gitxx {

ObjectId::ObjectId(const ObjectId::TagView tag) noexcept {
  std::ranges::copy(tag, m_id.begin());
}

std::string_view ObjectId::id() const noexcept {
  return std::string_view{m_id};
}

std::ostream& operator<<(std::ostream& stream,
                         const ObjectId& objectId) noexcept {
  return stream << objectId.id();
}

bool operator==(const ObjectId& lhs, const ObjectId& rhs) noexcept {
  return std::ranges::equal(lhs.m_id, rhs.m_id);
}

}  // namespace gitxx
