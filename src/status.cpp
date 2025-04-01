#include <libgit/repository.h>
#include <libgit/status.h>
#include <git2/status.h>
#include <iostream>
namespace git {


void    
StatusIterator::GitStatusListDeletor::operator()(git_status_list *ptr) const noexcept {
    if (ptr == nullptr) {
        return;
    }
    git_status_list_free(ptr);
}

StatusIterator::StatusIterator(Repository *repo, StatusOptions options) {

    git_status_options temp = GIT_STATUS_OPTIONS_INIT;

    git_status_list *statusList = nullptr;

    int res = git_status_list_new(&statusList, repo->m_repo.get(), &temp);
    if (res != 0) {
        throw std::runtime_error("Failed to create status list");
    }

    m_statusList = std::unique_ptr<git_status_list, GitStatusListDeletor>(statusList);
}

StatusIterator& StatusIterator::operator++() noexcept {
    m_index++;
    updateStatusEntry();
    return *this;
}

void StatusIterator::updateStatusEntry() noexcept {
    const git_status_entry *entry = git_status_byindex(m_statusList.get(), m_index);
    auto jeff = static_cast<int>(entry->status);
    std::cout << "jeff: " << jeff << std::endl;
    m_statusEntry.status = std::bitset<11>(entry->status);
}

StatusIterator::ReferenceType
StatusIterator::operator*() const noexcept {
    return m_statusEntry;
}

} // namespace git
