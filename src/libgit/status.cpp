#include <git2/errors.h>
#include <git2/status.h>

#include <flagfield.hpp>
#include <iostream>
#include <libgit/diff_delta.hpp>
#include <libgit/repository.hpp>
#include <libgit/status.hpp>
#include <libgit/status_options.hpp>

namespace {

git_status_options convertOptions(const git::StatusOptions &options) {
  return {.version = 1U,
          .show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR,
          .flags = options.flags.value(),
          .pathspec = {nullptr, 0U},
          .baseline = nullptr,
          .rename_threshold = options.rename_threshold};
}

git::DiffFile DiffFileFromGit2(const git_diff_file &file) {
  return git::DiffFile{
      .old_id{},
      .path{std::string(file.path)},
      .size = file.size,
      .flags = 0U,
      .mode = file.mode,
      .id_abbrev = 0U,
  };
}

std::optional<git::DiffDelta> DiffDeltaFromGit2(const git_diff_delta *delta) {
  if (delta == nullptr) {
    return std::nullopt;
  }

  return git::DiffDelta{
      .status = git::DiffDeltaStatus::Unmodified,
      .flags = delta->flags,
      .similarity = delta->similarity,
      .nfiles = delta->nfiles,
      .old_file = DiffFileFromGit2(delta->old_file),
      .new_file = DiffFileFromGit2(delta->new_file),
  };
}

}  // namespace

namespace git {
Status::Status(const Repository *repo, const StatusOptions &options) {
  git_status_list *status_list = nullptr;
  git_status_options opts = convertOptions(options);
  auto res = git_status_list_new(&status_list, repo->m_repo.get(), &opts);
  std::ignore = res;
}

StatusIterator Status::begin() const noexcept { return StatusIterator(*this); }

StatusIterator Status::end() const noexcept {
    StatusIterator end{*this};
    end.m_index = end.m_statusCount;
    return end;
}

}  // namespace git

namespace git {

StatusIterator::StatusIterator(const Status &status) {
  m_statusList = status.m_statusList;
  m_statusCount = git_status_list_entrycount(m_statusList.get());
  m_index = 0U;
}

StatusIterator &StatusIterator::operator++() noexcept {
  m_index++;
  updateStatusEntry();
  return *this;
}

void StatusIterator::updateStatusEntry() noexcept {
  if (m_index >= m_statusCount) {
    return;
  }

  const git_status_entry *entry =
      git_status_byindex(m_statusList.get(), m_index);
  if (entry == nullptr) {
    std::cout << "Entry is nullptr" << std::endl;
    return;
  }
  m_statusEntry.status = git::FlagField<git::FileStatus>{entry->status};
  m_statusEntry.head_to_index = DiffDeltaFromGit2(entry->head_to_index);
  m_statusEntry.index_to_workdir = DiffDeltaFromGit2(entry->index_to_workdir);
}

StatusIterator::ReferenceType StatusIterator::operator*() noexcept {
  return m_statusEntry;
}

}  // namespace git
