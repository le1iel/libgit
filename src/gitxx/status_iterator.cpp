#include <git2/status.h>

#include <algorithm>
#include <gitxx/status.hpp>

#include "flagfield.hpp"

namespace {

gitxx::DiffFile DiffFileFromGit2(const git_diff_file& file) {
  return gitxx::DiffFile{
      .old_id{},
      .path{std::string(file.path)},
      .size = file.size,
      .flags = gitxx::FlagField<gitxx::DiffFlag>{0U},
      .mode = file.mode,
  };
}

std::optional<gitxx::DiffDelta> DiffDeltaFromGit2(const git_diff_delta* delta) {
  if (delta == nullptr) {
    return std::nullopt;
  }

  return gitxx::DiffDelta{
      .status = gitxx::DiffDeltaStatus::Unmodified,
      .flags = gitxx::FlagField<gitxx::DiffFlag>{delta->flags},
      .similarity = delta->similarity,
      .nfiles = delta->nfiles,
      .old_file = DiffFileFromGit2(delta->old_file),
      .new_file = DiffFileFromGit2(delta->new_file),
  };
}

}  // namespace

namespace gitxx {

StatusIterator::StatusIterator(const Status& status)
    : m_index(0U),
      m_statusList(status.m_statusList),
      m_statusCount(
          m_statusList ? git_status_list_entrycount(m_statusList.get()) : 0U) {
  updateStatusEntry();
}

StatusIterator& StatusIterator::operator++() noexcept {
  ++m_index;
  updateStatusEntry();
  return *this;
}

StatusIterator StatusIterator::operator++(int) noexcept {
  StatusIterator temp = *this;
  ++(*this);
  return temp;
}

StatusIterator& StatusIterator::operator--() noexcept {
  if (m_index > 0) {
    --m_index;
    updateStatusEntry();
  }
  return *this;
}

StatusIterator StatusIterator::operator--(int) noexcept {
  StatusIterator temp = *this;
  --(*this);
  return temp;
}

// StatusIterator::ReferenceType StatusIterator::operator[](DifferenceType n)
// const noexcept {
//   auto idx = m_index + n;
//   StatusEntry entry;
//   if (idx < m_statusCount && m_statusList) {
//     const git_status_entry* s_entry = git_status_byindex(m_statusList.get(),
//     idx); if (s_entry) {
//       entry.status = FlagField<FileStatus>(s_entry->status);
//       entry.head_to_index = DiffDeltaFromGit2(s_entry->head_to_index);
//       entry.index_to_workdir = DiffDeltaFromGit2(s_entry->index_to_workdir);
//     }
//   }
//   // Returning by value since we can't return a reference to temporary.
//   return entry;
// }

StatusIterator StatusIterator::operator+(DifferenceType n) noexcept {
  StatusIterator tmp = *this;
  tmp.m_index += n;
  tmp.m_index = std::min(tmp.m_index, tmp.m_statusCount);
  tmp.updateStatusEntry();
  return tmp;
}

StatusIterator& StatusIterator::operator+=(DifferenceType n) noexcept {
  m_index += n;
  m_index = std::min(m_index, m_statusCount);
  updateStatusEntry();
  return *this;
}

StatusIterator StatusIterator::operator-(DifferenceType n) noexcept {
  StatusIterator tmp = *this;
  if (tmp.m_index < n) {
    tmp.m_index = 0;
  } else {
    tmp.m_index -= n;
  }
  tmp.updateStatusEntry();
  return tmp;
}

StatusIterator& StatusIterator::operator-=(DifferenceType n) noexcept {
  if (m_index < n) {
    m_index = 0;
  } else {
    m_index -= n;
  }
  updateStatusEntry();
  return *this;
}

StatusIterator::DifferenceType operator+(const StatusIterator& lhs,
                                         const StatusIterator& rhs) noexcept {
  // This is not standard meaning for container iterators - difference makes
  // sense, sum does not. Here, for compliance, may sum their indices BUT this
  // is not standard, so return index sum.
  return lhs.m_index + rhs.m_index;
}

StatusIterator::DifferenceType operator-(const StatusIterator& lhs,
                                         const StatusIterator& rhs) noexcept {
  return lhs.m_index - rhs.m_index;
}

void StatusIterator::updateStatusEntry() noexcept {
  if (m_index >= m_statusCount) {
    return;
  }

  const git_status_entry* entry =
      git_status_byindex(m_statusList.get(), m_index);
  if (entry == nullptr) {
    return;
  }
  m_statusEntry.status = gitxx::FlagField<gitxx::FileStatus>{entry->status};
  m_statusEntry.head_to_index = DiffDeltaFromGit2(entry->head_to_index);
  m_statusEntry.index_to_workdir = DiffDeltaFromGit2(entry->index_to_workdir);
}

StatusIterator::ReferenceType StatusIterator::operator*() noexcept {
  return m_statusEntry;
}

auto operator<=>(const StatusIterator& lhs,
                 const StatusIterator& rhs) noexcept {
  // Compare m_index first
  if (lhs.m_index < rhs.m_index) {
    return std::strong_ordering::less;
  }
  if (lhs.m_index > rhs.m_index) {
    return std::strong_ordering::greater;
  }

  // If m_index is equal, compare m_statusList pointers
  return lhs.m_statusList <=> rhs.m_statusList;
}

bool operator==(const StatusIterator& lhs, const StatusIterator& rhs) noexcept {
  return lhs.m_statusList == rhs.m_statusList;
}

}  // namespace gitxx
