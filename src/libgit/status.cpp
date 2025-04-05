#include <libgit/repository.h>
#include <libgit/status.h>
#include <libgit/diff_delta.h>
#include <git2/status.h>
#include <git2/errors.h>
#include <iostream>
#include <stdexcept>
#include "gtest/gtest.h"
#include <flagfield.h>

namespace {

    git_status_options convertOptions(const git::StatusOptions &options) {
        git_status_options opts = GIT_STATUS_OPTIONS_INIT;
        opts.version = 1U;
        // opts.show = static_cast<git_status_show_t>(options.show);
        std::cerr << "Flags: " << options.flags.value() << std::endl;
        opts.flags = options.flags.value();
        // opts.pathspec = options.pathspec.c_str();
        opts.rename_threshold = options.rename_threshold;
        return opts;
    }
    git::DiffFile DiffFileFromGit2(const git_diff_file &file) {
        return git::DiffFile {
            .path = std::string(file.path),
            .size = file.size,
            .mode = file.mode,
        };
    }

    std::optional<git::DiffDelta> DiffDeltaFromGit2(const git_diff_delta *delta) {
        if(delta == nullptr) {
            return std::nullopt;
        }

        return git::DiffDelta {
            .flags = delta->flags,
            .similarity = delta->similarity,
            .nfiles = delta->nfiles,
            .old_file = DiffFileFromGit2(delta->old_file),
            .new_file = DiffFileFromGit2(delta->new_file),
        };
    }
} // namespace detail

namespace git {

void
StatusIterator::GitStatusListDeletor::operator()(git_status_list *ptr) const noexcept {
    if (ptr == nullptr) {
        return;
    }
    git_status_list_free(ptr);
}

StatusIterator::StatusIterator(const Repository *repo, StatusOptions options) {
    if(repo == nullptr) {
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

    m_statusList = std::unique_ptr<git_status_list, GitStatusListDeletor>(statusList);

    m_statusCount = git_status_list_entrycount(m_statusList.get());
    updateStatusEntry();
}

StatusIterator& StatusIterator::operator++() noexcept {
    m_index++;
    updateStatusEntry();
    return *this;
}

void StatusIterator::updateStatusEntry() noexcept {
    if(m_index >= m_statusCount) {
        return;
    }

    const git_status_entry *entry = git_status_byindex(m_statusList.get(), m_index);
    if(entry == nullptr) {
        std::cout << "Entry is nullptr" << std::endl;
        return;
    }
    m_statusEntry.status = std::move(git::FlagField<git::FileStatus> {entry->status});

    m_statusEntry.head_to_index = DiffDeltaFromGit2(entry->head_to_index);
    m_statusEntry.index_to_workdir = DiffDeltaFromGit2(entry->index_to_workdir);
}

StatusIterator::ReferenceType
StatusIterator::operator*() const noexcept {
    return m_statusEntry;
}

} // namespace git
