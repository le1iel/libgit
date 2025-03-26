#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <git2/repository.h>
#include <optional>

#include "reference.h"

namespace git {

class Repository {
public:
  /// @brief Open from a path.
  /// @warning The path shall be null-terminated.
  static std::optional<Repository> Open(std::string_view path) noexcept;

  /// @brief Destructor.
  ~Repository();

  /// @brief Move constructable.
  Repository(Repository &&other);

  /// @brief Move assignable.
  Repository &operator=(Repository &&other);

  /// @brief Not copy constructable.
  Repository(const Repository &other) = delete;

  /// @brief Not copy assignable.
  Repository &operator=(const Repository &other) = delete;

  /// @brief returns the path of the repo.
  std::string path() const noexcept;

  /// @brief returns the head of the repo.
  std::optional<Reference> head() const noexcept;

private:
  Repository(std::string_view path, int *res) noexcept;

  struct GitRepositoryDeletor {
    void operator()(git_repository *ptr) {
      if (ptr == nullptr) {
        return;
      }
      git_repository_free(ptr);
    }
  };

  std::unique_ptr<git_repository, GitRepositoryDeletor> m_repo{nullptr};
};

} // namespace git

#endif
