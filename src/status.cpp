#include <libgit/repository.h>
#include <libgit/status.h>
#include <git2/status.h>
#include <iostream>
#include <stdexcept>
#include "flagfield.h"
#include "detail/diff_delta.h"
namespace git {

void    
StatusIterator::GitStatusListDeletor::operator()(git_status_list *ptr) const noexcept {
    if (ptr == nullptr) {
        return;
    }
    git_status_list_free(ptr);
}

StatusIterator::StatusIterator(const Repository *repo, StatusOptions options) {
    std::cout << "Making:" << std::endl;
    if(repo == nullptr) {
        throw std::runtime_error("Repository is nullptr");
    }

    m_index = 0;
    git_status_options temp = GIT_STATUS_OPTIONS_INIT;

    git_status_list *statusList = nullptr;

    int res = git_status_list_new(&statusList, repo->m_repo.get(), &temp);
    if (res != 0) {
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
    m_statusEntry.status = std::move(git::FlagField<git::FileStatus, 14> {entry->status});
    m_statusEntry.head_to_index = detail::DiffDeltaFromGit2(entry->head_to_index);
    m_statusEntry.index_to_workdir = detail::DiffDeltaFromGit2(entry->index_to_workdir);
}

StatusIterator::ReferenceType
StatusIterator::operator*() const noexcept {
    return m_statusEntry;
}

} // namespace git
