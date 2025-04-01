#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include "diff_delta.h"
#include "libgit/repository.h"
#include "flagfield.h"
#include <cstdint>
#include <string>


// forward declaration to hide libgit2 headers
struct git_status_list;

namespace git {

enum class FileStatus {
  Current=0,
  IndexNew=1,
  IndexModified=2,
  IndexDeleted=3,
  IndexRenamed=4,
  IndexTypeChanged=5,
  WtNew=7,
  WtModified=8,
  WtDeleted=9,
  WtTypeChange=10,
  WtRenamed=11,
  WtUnreadable=12,
  Ignored=13,
  Conflicted=14
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
  git::FlagField<git::FileStatus, 14> status {0};
  std::optional<DiffDelta> head_to_index;
  std::optional<DiffDelta> index_to_workdir;
};

class StatusIterator {
public:
    using ValueType = StatusEntry;
    using PointerType = ValueType*;
    using ReferenceType = const ValueType&;

    /// @brief Constructor.
    StatusIterator(const Repository *repo, StatusOptions options);

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

  std::size_t m_statusCount;

  StatusEntry m_statusEntry;

  std::unique_ptr<git_status_list, GitStatusListDeletor> m_statusList;
};

} // namespace git
#endif
