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

template<typename Allocator = std::allocator<std::byte>>
class BasicRepository {
 public:
     using char_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<char>;
     using string_type = std::basic_string<char, std::char_traits<char>, char_allocator>;

  /// @brief Open from a path.
  [[nodiscard]] static std::expected<BasicRepository, GitErrc> Open(
      const std::filesystem::path path) noexcept;

  /// @brief Destructor.
  ~BasicRepository() = default;

  /// @brief Move constructable.
  BasicRepository(BasicRepository &&other) = default;

  /// @brief Move assignable.
  BasicRepository &operator=(BasicRepository &&other) = default;

  /// @brief Copy constructable.
  BasicRepository(const BasicRepository &other) = default;

  /// @brief Not copy assignable.
  BasicRepository &operator=(const BasicRepository &other) = default;

  /// @brief returns the path of the repo.
  /// @detail std::filesystem::path does not support custom allocators, so
  ///     path is not supported.
  [[nodiscard]] string_type path() const noexcept;

  /// @brief returns the head of the repo.
  [[nodiscard]] std::optional<Reference> head() const noexcept;

  /// @brief returns the status of the repo.
  [[nodiscard]] std::expected<Status, GitErrc> status() const noexcept;

  /// @brief returns the status of the repo.
  [[nodiscard]] std::expected<Status, GitErrc> status(StatusOptions options) const noexcept;

  friend class Status;

 private:
  /// @brief Private constructor so that error handling can be done.
  /// @warning The path shall be null-terminated.
  /// @param path The path to the repository.
  /// @param res The result of the operation.
  BasicRepository(std::string_view path, int *res) noexcept;

  /// @brief The libgit2 repository object.
  std::shared_ptr<git_repository> m_repo{nullptr};
};

using Repository = BasicRepository<>;

}  // namespace gitxx

#endif  // INCLUDE_GITXX_REPOSITORY_HPP_
