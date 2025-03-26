#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include "libgit/repository.h"
#include <cstdint>
#include <git2/status.h>
#include <string>

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

class StatusEntry {
private:
  StatusEntry(git_status_entry &entry);
};

class StatusIterator {
public:
private:
  StatusIterator(Repository *repo, StatusOptions options);

  std::unique_ptr<git_status_list> m_statusList;
};

} // namespace git
#endif
