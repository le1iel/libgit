#include <git2/diff.h>
#include <git2/status.h>
#include <git2/types.h>

#include <algorithm>
#include <converters/diff_delta.hpp>
#include <converters/status_options.hpp>
#include <cstddef>
#include <flagfield.hpp>
#include <gitxx/diff_delta.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>
#include <memory>
#include <ranges>
#include <string_view>

namespace gitxx {

template <typename Allocator>
Status::Status(const BasicRepository<Allocator>* repo,
               const StatusOptions& options, int* res) {
  git_status_list* status_list = nullptr;
  const git_status_options opts =
      internal::conversion_traits<git_status_options,
                                  gitxx::StatusOptions>::from_cpp(options);

  auto resOut = git_status_list_new(&status_list, repo->m_repo.get(), &opts);

  if (resOut != 0) {
    *res = -1;
    return;
  }
  m_statusList =
      std::shared_ptr<git_status_list>(status_list, git_status_list_free);
}

StatusEntry Status::file(std::string_view file) const noexcept {
  StatusEntry result;

  if (!m_statusList) {
    return result;
  }

  auto entries =
      std::views::iota(
          std::size_t{0},
          git_status_list_entrycount(
              m_statusList.get())) |  // for each index perform this transform
      std::views::transform([this](size_t index) {
        return git_status_byindex(m_statusList.get(), index);
      })
      // for each entry filter our null entries
      | std::views::filter(
            [](const git_status_entry* entry) { return entry != nullptr; });

  auto entry_it =
      std::ranges::find_if(entries, [&file](const git_status_entry* entry) {
        const git_diff_delta* delta = nullptr;
        if (entry->head_to_index != nullptr) {
          delta = entry->head_to_index;
        }
        if (entry->index_to_workdir != nullptr) {
          delta = entry->index_to_workdir;
        }

        return (delta != nullptr && delta->new_file.path != nullptr &&
                (file == delta->new_file.path));
      });

  if (entry_it != entries.end()) {
    const auto* entry = *entry_it;
    result.status = gitxx::FlagField<gitxx::FileStatus>(entry->status);
    result.head_to_index =
        internal::conversion_traits<gitxx::DiffDelta, git_diff_delta>::from_c(
            entry->head_to_index);
    result.index_to_workdir =
        internal::conversion_traits<gitxx::DiffDelta, git_diff_delta>::from_c(
            entry->index_to_workdir);
    return result;
  }

  return result;
}

StatusIterator Status::begin() const noexcept { return StatusIterator(*this); }

StatusIterator Status::end() const noexcept {
  StatusIterator end{*this};
  end.m_index = end.m_statusCount;
  return end;
}

// Explicit template instantiation for the Status constructor with the
// BasicRepository type used in tests
template Status::Status(const BasicRepository<std::allocator<std::byte>>* repo,
                        const StatusOptions& options, int* res);

}  // namespace gitxx
