#ifndef INCLUDE_GITXX_STATUS_OPTIONS_HPP_
#define INCLUDE_GITXX_STATUS_OPTIONS_HPP_

#include <cstdint>
#include <flagfield.hpp>
#include <string>

namespace git {

enum class StatusShow {
  IndexAndWorkdir = 0,
  Index,
  Workdir,
};

enum class StatusFlags {
  IncludeUntracked = 0,
  IncludeIgnored,
  IncludeUnmodified,
  ExcludeSubmodules,
  RecurseUntrackedDirs,
  DisablePathspecMatch,
  RecurseIgnoredDirs,
  RenamesHeadToIndex,
  RenamesIndexToWorkdir,
  SortCaseSensitively,
  SortCaseInsensitively,
  RenamesFromRewrites,
  NoRefresh,
  UpdateIndex,
  IncludeUnreadable,
  IncludeUnreadableAsUntracked,
};

struct StatusOptions {
  std::uint16_t version;
  StatusShow show;
  git::FlagField<StatusFlags> flags;
  std::string pathspec;
  // Tree* tree; TODO
  std::uint16_t rename_threshold;
};

}  // namespace git

#endif  // INCLUDE_GITXX_STATUS_OPTIONS_HPP_
