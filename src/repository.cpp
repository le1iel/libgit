#include <git2/repository.h>
#include <libgit/repository.h>
#include <memory>
#include <optional>

namespace git {

std::optional<git::Repository>
Repository::Open(std::string_view path) noexcept {
  int res = 0;
  Repository repo{path, &res};
  if (res != 0) {
    return std::nullopt;
  }
  return repo;
}

Repository::Repository(std::string_view path, int *resOut) noexcept {
  git_repository *repo = nullptr;
  auto openRes = git_repository_open(&repo, path.cbegin());

  if (openRes != 0) {
    *resOut = -1;
    return;
  }

  auto out =
      std::unique_ptr<git_repository, Repository::GitRepositoryDeletor>(repo);
  m_repo = std::move(out);
}

Repository::Repository(Repository &&other) : m_repo(other.m_repo.release()) {}

Repository &Repository::operator=(Repository &&other) {
  m_repo.swap(other.m_repo);
  return *this;
}

Repository::~Repository() { m_repo.reset(); }

std::string Repository::path() const noexcept {
  std::string path;
  if (m_repo) {
    path = std::string(git_repository_path(m_repo.get()));
  }

  return path;
}

}; // namespace git
