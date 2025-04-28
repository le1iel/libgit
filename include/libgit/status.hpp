#ifndef INCLUDE_LIBGIT_STATUS_H_
#define INCLUDE_LIBGIT_STATUS_H_

#include <libgit/diff_delta.hpp>
#include <flagfield.hpp>
#include <libgit/repository.hpp>
#include <libgit/status_options.hpp>

// forward declaration to hide libgit2 headers
struct git_status_list;

namespace git {

/// @brief Enum representing the status of a file in the repository.
enum class FileStatus {
  ///@brief No changes.
  Current = 0,
  ///@brief New in index.
  IndexNew = 1,
  ///@brief Modified in index.
  IndexModified = 2,
  ///@brief Deleted in index.
  IndexDeleted = 3,
  ///@brief Renamed in index.
  IndexRenamed = 4,
  ///@brief Type changed in index.
  IndexTypeChanged = 5,
  ///@brief New in workdir.
  WtNew = 7,
  ///@brief Modified in workdir.
  WtModified = 8,
  ///@brief Deleted in workdir.
  WtDeleted = 9,
  ///@brief Type changed in workdir.
  WtTypeChange = 10,
  ///@brief Renamed in workdir.
  WtRenamed = 11,
  ///@brief Unreadable in workdir.
  WtUnreadable = 12,
  ///@brief File in ignored.
  Ignored = 13,
  ///@brief File is conflicted.
  Conflicted = 14
};

struct StatusEntry {
  git::FlagField<git::FileStatus> status{0};
  std::optional<DiffDelta> head_to_index;
  std::optional<DiffDelta> index_to_workdir;
};

class StatusIterator {
 public:
  using IteratorCategory = std::bidirectional_iterator_tag;
  using ValueType = StatusEntry;
  using PointerType = ValueType*;
  using ReferenceType = const ValueType&;

  /// @brief Constructor.
  StatusIterator(const Repository* repo, StatusOptions options);

  /// @brief Pre-increment operator.
  StatusIterator& operator++() noexcept;

  /// @brief Post-increment operator.
  StatusIterator operator++(int) noexcept;

  /// @brief Pre-decrement operator.
  StatusIterator& operator--() noexcept;

  /// @brief Post-decrement operator.
  StatusIterator operator--(int) noexcept;

  /// @brief Dereference operator.
  ReferenceType operator*() const noexcept;

  operator bool() const noexcept ;

  friend bool operator==(const StatusIterator lhs, const StatusIterator rhs) noexcept;

  friend bool operator!=(const StatusIterator lhs, const StatusIterator rhs) noexcept;

 private:
  /// @brief Deleter for the status list.
  struct GitStatusListDeletor {
    void operator()(git_status_list* list) const noexcept;
  };

  /// @brief Updates the status entry.
  void updateStatusEntry() noexcept;

  /// @brief The index of the current status entry.
  std::size_t m_index;

  /// @brief The number of status entries.
  std::size_t m_statusCount;

  /// @brief The current status entry.
  StatusEntry m_statusEntry;

  /// @brief The status list.
  std::unique_ptr<git_status_list, GitStatusListDeletor> m_statusList;
};

}  // namespace git
#endif  // INCLUDE_LIBGIT_STATUS_H_
