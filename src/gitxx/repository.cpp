
#include <git2/global.h>
#include <git2/repository.h>
#include <git2/types.h>

#include <converters/error.hpp>
#include <cstddef>
#include <expected>
#include <filesystem>
#include <gitxx/error.hpp>
#include <gitxx/reference.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace gitxx {

template <typename Allocator>
std::expected<BasicRepository<Allocator>, GitErrc>
BasicRepository<Allocator>::Open(const std::filesystem::path path) noexcept {
  using char_allocator =
      typename std::allocator_traits<Allocator>::template rebind_alloc<char>;
  const char_allocator alloc{};

  const auto path_str =
      path.string<char, std::char_traits<char>, char_allocator>(alloc);
  int resOut = 0U;

  BasicRepository repo(path_str, &resOut);
  if (resOut != 0) {
    return std::unexpected<GitErrc>(
        internal::conversion_traits<GitErrc, int>::from_c(resOut));
  }

  return repo;
}

template <typename Allocator>
BasicRepository<Allocator>::BasicRepository(std::string_view path,
                                            int* resOut) noexcept {
  git_libgit2_init();

  git_repository* repo = nullptr;

  const int openRes = git_repository_open(&repo, path.cbegin());

  if (openRes != 0) {
    *resOut = openRes;
    git_libgit2_shutdown();
    return;
  }

  m_repo = std::shared_ptr<git_repository>(repo, [](git_repository* raw) {
    git_repository_free(raw);
    git_libgit2_shutdown();
  });
}

template <typename Allocator>
BasicRepository<Allocator>::string_type BasicRepository<Allocator>::path()
    const noexcept {
  BasicRepository<Allocator>::string_type path;
  if (m_repo) {
    path = git_repository_path(m_repo.get());
  }

  return path;
}

template <typename Allocator>
std::optional<Reference> BasicRepository<Allocator>::head() const noexcept {
  git_reference* ref = nullptr;

  const int res = git_repository_head(&ref, m_repo.get());
  if (res != 0) {
    return std::nullopt;
  }

  return Reference{ref};
}

template <typename Allocator>
std::expected<Status, GitErrc> BasicRepository<Allocator>::status()
    const noexcept {
  return status(StatusOptions{});
}

template <typename Allocator>
std::expected<Status, GitErrc> BasicRepository<Allocator>::status(
    StatusOptions options) const noexcept {
  int res{0};
  auto status = Status{this, options, &res};

  if (res == 0) {
    return status;
  } else {
    return std::unexpected(
        internal::conversion_traits<GitErrc, int>::from_c(res));
  }
}

// Explicit template instantiation for the type used in tests
template class BasicRepository<std::allocator<std::byte>>;

};  // namespace gitxx
