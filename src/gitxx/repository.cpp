
#include <git2/errors.h>
#include <git2/repository.h>

#include <converters.hpp>
#include <expected>
#include <gitxx/error.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <log/logger.hpp>
#include <memory>
#include <optional>

using gitxx::log::Log;

namespace gitxx {

// template<typename Allocator = std::allocator<void>>
std::expected<gitxx::Repository, GitErrc> Repository::Open(
    const std::filesystem::path path) noexcept {
  const auto path_str = path.string();
  int resOut = 0U;

  Repository repo(path_str, &resOut);
  if (resOut != 0) {
    return std::unexpected<GitErrc>(internal::Error(git_error_last()->klass));
  }

  return repo;
}

Repository::Repository(std::string_view path, int *resOut) noexcept {
  git_repository *repo = nullptr;

  int openRes = git_repository_open(&repo, path.cbegin());

  if (openRes != 0) {
    Log::error(git_error_last()->message);
    *resOut = -1;
    return;
  }

  m_repo = std::shared_ptr<git_repository>(repo, [](git_repository *ptr) {
    if (ptr == nullptr) {
      return;
    }
    git_repository_free(ptr);
  });
}

std::string Repository::path() const noexcept {
  std::string path;
  if (m_repo) {
    path = std::string(git_repository_path(m_repo.get()));
  }

  return path;
}

std::optional<Reference> Repository::head() const noexcept {
  git_reference *ref = nullptr;

  int res = git_repository_head(&ref, m_repo.get());
  if (res != 0) {
    return std::nullopt;
  }

  return Reference{ref};
}

std::expected<Status, GitErrc> Repository::status() const noexcept {
  return status(StatusOptions{});
}

std::expected<Status, GitErrc> Repository::status(
    StatusOptions options) const noexcept {
  int res{0};
  auto status = Status{this, options, &res};

  if (res == 0) {
    return status;
  } else {
    return std::unexpected(internal::Error(res));
  }
}

};  // namespace gitxx
