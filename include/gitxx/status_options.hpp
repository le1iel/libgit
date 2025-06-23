#ifndef INCLUDE_GITXX_STATUS_OPTIONS_HPP_
#define INCLUDE_GITXX_STATUS_OPTIONS_HPP_

#include <cstdint>
#include <flagfield.hpp>
#include <gitxx/tree.hpp>
#include <string>
#include <vector>

namespace gitxx {

/// @brief Files on which to report status.
enum class StatusShow {
  /// @brief The default. This roughly matches `git status --porcelain`
  ///        regarding which files are included and in what order.
  IndexAndWorkdir = 0,
  /// @brief Only gives status based on HEAD to index comparison, not looking at
  ///        working directory changes.
  Index,
  /// @brief Only gives status based on index to working directory comparison,
  ///       not comparing the index to the HEAD.
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

/// @brief Options to control how `Repository::status()` works.
struct StatusOptions {
  /// @brief The struct version.
  std::uint16_t version;

  /// @brief The show value that control which files to scan and in what order.
  StatusShow show = StatusShow::Workdir;

  /// @brief Flags describing which changes should be considered.
  gitxx::FlagField<StatusFlags> flags{StatusFlags::IncludeIgnored,
                                      StatusFlags::IncludeUntracked,
                                      StatusFlags::RecurseUntrackedDirs};
  /// @brief The pathspec is an vector of path patterns to match (using
  ///        fnmatch-style matching), or just an vector of paths to match exactly
  ///        if `StatusFlags::DisablePathspecMatch` is specified in the flags.
  std::vector<std::string> pathspec{};

  /// @brief The baseline is the tree to be used for comparison to the working
  ///        directory and index; defaults to HEAD.
  Tree baseline{};

  /// @brief Threshold above which similar files will be considered renames.
  ///        This is equivalent to the -M option. Defaults to 50.
  std::uint16_t rename_threshold;
};

}  // namespace gitxx

#endif  // INCLUDE_GITXX_STATUS_OPTIONS_HPP_
