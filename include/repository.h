#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <git2/errors.h>
#include <git2/refs.h>
#include <git2/repository.h>
#include <optional>
#include <string_view>

#include "reference.h"
#include "status.h"
#include "libgit/export_libgit.hpp"

class LIBGIT_EXPORT Repo {
public:
  /// @brief Open a repository from a path.
  /// @warning Needs to be null-terminated.
  static std::optional<Repo> Open(std::string_view path);

  /// @brief returns the path of the repo.
  std::string path() const noexcept;

  std::optional<Reference> head() const noexcept;

  std::optional<Status> status() const noexcept;

  git_repository *getPointer() noexcept { return m_repo.get(); }

private:
  Repo(std::string_view path, int &res);

  struct GitRepoDeletor {
    void operator()(git_repository *repo) { git_repository_free(repo); };
  };

  std::unique_ptr<git_repository, GitRepoDeletor> m_repo;
};

#endif
