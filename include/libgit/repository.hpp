#ifndef INCLUDE_LIBGIT_REPOSITORY_H_
#define INCLUDE_LIBGIT_REPOSITORY_H_

#include <expected>
#include <optional>
#include <libgit/error.hpp>
#include <libgit/reference.hpp>
#include <libgit/status_options.hpp>

// forward declaration to hide libgit2 headers
struct git_repository;

namespace git {

class Status;

class Repository {
 public:
  /// @brief Open from a path.
  /// @warning The path shall be null-terminated.
  static std::expected<Repository, GitErrc> Open(
      std::string_view path) noexcept;

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

  /// @brief returns the status of the repo.
  Status status() const noexcept;

  /// @brief returns the status of the repo.
  Status status(StatusOptions options) const noexcept;

  friend class Status;

 private:
  /// @brief Private constructor so that error handling can be done.
  /// @warning The path shall be null-terminated.
  /// @param path The path to the repository.
  /// @param res The result of the operation.
  Repository(std::string_view path, int *res) noexcept;

  /// @brief Deleter for the repository.
  struct GitRepositoryDeletor {
    void operator()(git_repository *ptr) const noexcept;
  };

  /// @brief The libgit2 repository object.
  std::unique_ptr<git_repository, GitRepositoryDeletor> m_repo{nullptr};
};

}  // namespace git

#endif  // INCLUDE_LIBGIT_REPOSITORY_H_
