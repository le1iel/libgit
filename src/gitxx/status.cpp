#include <git2/errors.h>
#include <git2/status.h>

#include <compare>
#include <flagfield.hpp>
#include <gitxx/diff_delta.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>

namespace {

/// @brief Converts internal options struct to libgit2s struct
git_status_options convertOptions(const gitxx::StatusOptions &options) {
  return {.version = 1U,
          .show = static_cast<git_status_show_t>(options.show),
          .flags = options.flags.value(),
          .pathspec = {nullptr, 0U},
          .baseline = nullptr,
          .rename_threshold = options.rename_threshold};
}

gitxx::DiffFile DiffFileFromGit2(const git_diff_file &file) {
  return gitxx::DiffFile{
      .old_id{},
      .path{std::string(file.path)},
      .size = file.size,
      .flags = 0U,
      .mode = file.mode,
      .id_abbrev = 0U,
  };
}

std::optional<gitxx::DiffDelta> DiffDeltaFromGit2(const git_diff_delta *delta) {
  if (delta == nullptr) {
    return std::nullopt;
  }

  return gitxx::DiffDelta{
      .status = gitxx::DiffDeltaStatus::Unmodified,
      .flags = delta->flags,
      .similarity = delta->similarity,
      .nfiles = delta->nfiles,
      .old_file = DiffFileFromGit2(delta->old_file),
      .new_file = DiffFileFromGit2(delta->new_file),
  };
}

}  // namespace

namespace gitxx {


template<typename Allocator>
Status::Status(const BasicRepository<Allocator> *repo, const StatusOptions &options, int *res) {
  git_status_list *status_list = nullptr;
  git_status_options opts = convertOptions(options);

  auto resOut = git_status_list_new(&status_list, repo->m_repo.get(), &opts);

  if (resOut != 0) {
    *res = -1;
    return;
  }
  m_statusList =
      std::shared_ptr<git_status_list>(status_list, git_status_list_free);
}

void Status::GitStatusListDeletor::operator()(
    git_status_list *list) const noexcept {
  if (list) git_status_list_free(list);
}

StatusEntry Status::file(std::string_view file) const noexcept {
  StatusEntry result;

  if (!m_statusList) return result;

  size_t count = git_status_list_entrycount(m_statusList.get());

  for (size_t i = 0; i < count; ++i) {
    const git_status_entry *entry = git_status_byindex(m_statusList.get(), i);
    if (!entry) continue;

    const git_diff_delta *delta =
        entry->head_to_index
            ? entry->head_to_index
            : (entry->index_to_workdir ? entry->index_to_workdir : nullptr);
    if (delta && delta->new_file.path && file == delta->new_file.path) {
      result.status = gitxx::FlagField<gitxx::FileStatus>{entry->status};
      result.head_to_index = DiffDeltaFromGit2(entry->head_to_index);
      result.index_to_workdir = DiffDeltaFromGit2(entry->index_to_workdir);
      return result;
    }
  }
  return result;
}

StatusIterator Status::begin() const noexcept { return StatusIterator(*this); }

StatusIterator Status::end() const noexcept {
  StatusIterator end{*this};
  end.m_index = end.m_statusCount;
  return end;
}

// Custom deleter for git_status_list

auto operator<=>(const std::shared_ptr<git_status_list> lhs,
                 const std::shared_ptr<git_status_list> rhs) {
  if (!lhs && !rhs) return std::strong_ordering::equivalent;
  if (!lhs) return std::strong_ordering::less;
  // if (!rhs)
  return std::strong_ordering::greater;
}

// Explicit template instantiation for the Status constructor with the BasicRepository type used in tests
template Status::Status(const BasicRepository<std::allocator<std::byte>>* repo, const StatusOptions& options, int* res);

}  // namespace gitxx
