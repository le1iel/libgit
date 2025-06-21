#ifndef INCLUDE_GITXX_REPOSITORY_HPP_
#define INCLUDE_GITXX_REPOSITORY_HPP_

#include <expected>
#include <filesystem>
#include <gitxx/error.hpp>
#include <gitxx/reference.hpp>
#include <gitxx/status_options.hpp>
#include <optional>

// forward declaration to hide libgit2 headers
struct git_repository;

namespace gitxx {

class Status;

// template<typename Allocator = std::allocator<void>>
class Repository {
 public:
  /// @brief Open from a path.
  static std::expected<Repository, GitErrc> Open(
      const std::filesystem::path path) noexcept;

  /// @brief Destructor.
  ~Repository() = default;

  /// @brief Move constructable.
  Repository(Repository &&other) = default;

  /// @brief Move assignable.
  Repository &operator=(Repository &&other) = default;

  /// @brief Copy constructable.
  Repository(const Repository &other) = default;

  /// @brief Not copy assignable.
  Repository &operator=(const Repository &other) = default;

  /// @brief returns the path of the repo.
  std::string path() const noexcept;

  /// @brief returns the head of the repo.
  std::optional<Reference> head() const noexcept;

  /// @brief returns the status of the repo.
  std::expected<Status, GitErrc> status() const noexcept;

  /// @brief returns the status of the repo.
  std::expected<Status, GitErrc> status(StatusOptions options) const noexcept;

  friend class Status;

 private:
  // using AllocTraits = std::allocator_traits<Allocator>;

  // Allocator m_alloc;

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
  std::shared_ptr<git_repository> m_repo{nullptr};
};

}  // namespace gitxx

#endif  // INCLUDE_GITXX_REPOSITORY_HPP_
