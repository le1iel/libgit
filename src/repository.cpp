#include <optional>
#include <string>

#include <reference.h>
#include <repository.h>

std::optional<Repo> Repo::Open(std::string_view path) {
  int res{-1};
  Repo r{path, res};
  if (res != 0) {
    return std::nullopt;
  }
  return r;
}

Repo::Repo(std::string_view path, int &res) {
  git_repository *repo = nullptr;
  int openRes = git_repository_open(&repo, path.begin());

  if (openRes < 0) {
    return;
  }
  m_repo = std::unique_ptr<git_repository, GitRepoDeletor>(repo);

  git_reference *ref = nullptr;
  res = git_repository_head(&ref, m_repo.get());
  if (res < 0) {
    return;
  }
  res = 0;
}

std::string Repo::path() const noexcept {
  std::string path;
  if (m_repo) {

    path = std::string(git_repository_path(m_repo.get()));
  }
  return path;
}

std::optional<Reference> Repo::head() const noexcept {
  git_reference *ref = nullptr;
  int res = git_repository_head(&ref, m_repo.get());
  if (res < 0) {
    return std::nullopt;
  }

  return Reference{ref};
}
