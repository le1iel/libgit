#ifndef GIT_STATUS_H
#define GIT_STATUS_H

#include <git2/status.h>

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

class Status {};

class StatusEntry {
  StatusEntry(git_status_entry &entry);
};

} // namespace git
#endif
