#ifndef GIT_STATUS_H
#define GIT_STATUS_H
#include <git2/status.h>
#include <string_view>

enum class FileStatus
{
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


class Status
{

    static Status FromFile(std::string_view path);
};

#endif
