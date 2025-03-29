#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include "libgit/repository.h"
#include <cstdint>
#include <string>

// forward declaration to hide libgit2 headers
struct git_status_entry;
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

class StatusIterator;

class StatusEntry {
public:
    using Iterator = StatusIterator;

  StatusEntry(git_status_entry &entry);
private:
};

class StatusIterator {
public:
    using ValueType = StatusEntry;
    using PointerType = ValueType*;
    using ReferenceType = ValueType&;

    /// @brief Constructor.
    StatusIterator(Repository *repo, StatusOptions options);

    /// @brief Pre-increment operator.
    StatusIterator& operator++() noexcept;

    /// @brief Post-increment operator.
    StatusIterator& operator++(int) noexcept;

    /// @brief Dereference operator.
    ReferenceType operator*() const noexcept;
private:
  /// @brief Deleter for the status list.
  struct GitStatusListDeletor {
    void operator()(git_status_list *list) const noexcept;
  };

  std::unique_ptr<git_status_list, GitStatusListDeletor> m_statusList;
};

} // namespace git
#endif
