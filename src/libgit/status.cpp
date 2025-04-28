#include <iostream>
#include <stdexcept>

#include <git2/errors.h>
#include <git2/status.h>

#include <flagfield.hpp>
#include <libgit/diff_delta.hpp>
#include <libgit/repository.hpp>
#include <libgit/status.hpp>

namespace {

git_status_options convertOptions(const git::StatusOptions &options) {
  git_status_options opts{
      .version = 1U,
      .show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR,
      .flags = 0U,
      .pathspec = {nullptr, 0U},
      .baseline = nullptr,
      .rename_threshold = 50
  };
  // opts.show = static_cast<git_status_show_t>(options.show);
  std::cerr << "Flags: " << options.flags.value() << std::endl;
  opts.flags = options.flags.value();
  // opts.pathspec = options.pathspec.c_str();
  opts.rename_threshold = options.rename_threshold;
  return opts;
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

void StatusIterator::GitStatusListDeletor::operator()(
    git_status_list *ptr) const noexcept {
  if (ptr == nullptr) {
    return;
  }
  git_status_list_free(ptr);
}

StatusIterator::StatusIterator(const Repository *repo, StatusOptions options) {
  if (repo == nullptr) {
    throw std::runtime_error("Repository is nullptr");
  }

  m_index = 0;
  git_status_options temp = convertOptions(options);

  git_status_list *statusList = nullptr;

  int res = git_status_list_new(&statusList, repo->m_repo.get(), &temp);
  if (res != 0) {
    // ToDo Errors
    throw std::runtime_error("Failed to create status list");
  }

  m_statusList =
      std::unique_ptr<git_status_list, GitStatusListDeletor>(statusList);

  m_statusCount = git_status_list_entrycount(m_statusList.get());
  updateStatusEntry();
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

<<<<<<< HEAD
    libgit::log::Log::debug("hello");
    const git_status_entry *entry = git_status_byindex(m_statusList.get(), m_index);
    if(entry == nullptr) {
        libgit::log::Log::error("Entry is nullptr");
        // std::cout << "Entry is nullptr" << std::endl;
        return;
    }
    m_statusEntry.status = std::move(git::FlagField<git::FileStatus> {entry->status});
=======
  const git_status_entry *entry =
      git_status_byindex(m_statusList.get(), m_index);
  if (entry == nullptr) {
    std::cout << "Entry is nullptr" << std::endl;
    return;
  }
  m_statusEntry.status = git::FlagField<git::FileStatus>{entry->status};
>>>>>>> develop

  m_statusEntry.head_to_index = DiffDeltaFromGit2(entry->head_to_index);
  m_statusEntry.index_to_workdir = DiffDeltaFromGit2(entry->index_to_workdir);
}

<<<<<<< HEAD
StatusIterator::operator bool() const noexcept
{
    if(not m_statusList)
    {
        return false;
    }

    if (m_index >= m_statusCount)
    {
        return false;
    }

    return true;

}


StatusIterator::ReferenceType
StatusIterator::operator*() const noexcept {
    return m_statusEntry;
=======
StatusIterator::ReferenceType StatusIterator::operator*() const noexcept {
  return m_statusEntry;
>>>>>>> develop
}

}  // namespace git
