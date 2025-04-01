#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include "diff_delta.h"
#include "libgit/repository.h"
#include <cstdint>
#include <string>
#include <bitset>


// forward declaration to hide libgit2 headers
struct git_status_list;

namespace git {

enum class FileStatus {
  FileError = 0,
  Current,
  IndexNew,
  IndexModified,
  IndexDeleted,
  IndexRenamed,
  IndexTypeChanged,
  WtName,
  WtModified,
  WtDeleted,
  WtTypeChange,
  WtRenamed,
  WtUnreadable,
  Ignored,
  Conflicted
};

enum class StatusShow {
  IndexAndWorkdir = 0,
  Index,
  Workdir,
};

struct StatusOptions {
  std::uint16_t version;
  StatusShow show;
  std::uint16_t flags;
  std::string pathspec;
  // Tree* tree; TODO
  std::uint16_t rename_threshold;
};


struct StatusEntry {
  std::bitset<11> status;
  DiffDelta* head_to_index;
  DiffDelta* index_to_workdir;
};

class StatusIterator {
public:
    using ValueType = StatusEntry;
    using PointerType = ValueType*;
    using ReferenceType = const ValueType&;

    /// @brief Constructor.
    StatusIterator(Repository *repo, StatusOptions options);

    /// @brief Pre-increment operator.
    StatusIterator& operator++() noexcept;

    /// @brief Post-increment operator.
    StatusIterator operator++(int) noexcept;

    /// @brief Dereference operator.
    ReferenceType operator*() const noexcept;
private:
  /// @brief Deleter for the status list.
  struct GitStatusListDeletor {
    void operator()(git_status_list *list) const noexcept;
  };

  /// @brief Updates the status entry.
  void updateStatusEntry() noexcept;

  std::size_t m_index;

  StatusEntry m_statusEntry;

  std::unique_ptr<git_status_list, GitStatusListDeletor> m_statusList;
};

} // namespace git
#endif
