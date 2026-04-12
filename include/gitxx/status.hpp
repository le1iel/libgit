#ifndef INCLUDE_GITXX_STATUS_HPP_
#define INCLUDE_GITXX_STATUS_HPP_

#include <cstddef>
#include <cstdint>
#include <flagfield.hpp>
#include <gitxx/diff_delta.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status_options.hpp>
#include <iterator>
#include <memory>
#include <optional>
#include <string_view>

// forward declaration to hide libgit2 headers
struct git_status_list;

namespace gitxx {

/// @brief Enum representing the status of a file in the repository.
enum class FileStatus : std::uint8_t {
  /// @brief No changes.
  Current = 0,
  /// @brief New in index.
  IndexNew = 0,
  /// @brief Modified in index.
  IndexModified = 1,
  /// @brief Deleted in index.
  IndexDeleted = 2,
  /// @brief Renamed in index.
  IndexRenamed = 3,
  /// @brief Type changed in index.
  IndexTypeChanged = 4,
  /// @brief New in workdir.
  WtNew = 7,
  /// @brief Modified in workdir.
  WtModified = 8,
  /// @brief Deleted in workdir.
  WtDeleted = 9,
  /// @brief Type changed in workdir.
  WtTypeChange = 10,
  /// @brief Renamed in workdir.
  WtRenamed = 11,
  /// @brief Unreadable in workdir.
  WtUnreadable = 12,
  /// @brief File in ignored.
  Ignored = 14,
  /// @brief File is conflicted.
  Conflicted = 15
};

/// @brief A status entry, providing the differences between the file as it
///        exists in HEAD and the index, and providing the differences between
///        the index and the working directory.
struct StatusEntry {
  /// @brief status flags for this file.
  gitxx::FlagField<gitxx::FileStatus> status{0};
  /// @brief Detailed information about the differences between the file in HEAD
  ///        and the file in the index.
  std::optional<DiffDelta> head_to_index;
  /// @brief Detailed information about the differences between the file in the
  ///        index and the file in the working directory.
  std::optional<DiffDelta> index_to_workdir;
};

// forward declaration
class StatusIterator;

/// @brief Status indicates how a user has changed the working directory and
///        index.
class Status {
  using IteratorType = StatusIterator;

 public:
  /// @brief Destructor.
  ~Status() = default;

  /// @brief Move constructable.
  Status(Status&& other) = default;

  /// @brief Copy constructable.
  Status(const Status& other) = default;

  /// @brief Move assignable.
  Status& operator=(Status&& other) = default;

  /// @brief Copy assignable.
  Status& operator=(const Status& other) = default;

  /// @brief Beginning iterator.
  [[nodiscard]] IteratorType begin() const noexcept;

  /// @brief End iterator.
  [[nodiscard]] IteratorType end() const noexcept;

  /// @brief Get the status for the given file.
  [[nodiscard]] StatusEntry file(std::string_view file) const noexcept;

  template <typename Allocator>
  friend class BasicRepository;
  friend class StatusIterator;

 private:
  /// @brief Private constructor.
  template <typename Allocator>
  Status(const BasicRepository<Allocator>* repo, const StatusOptions& options,
         int* res);

  /// @brief The status list.
  std::shared_ptr<git_status_list> m_statusList;
};

/// @brief A random-access iterator for navigating git status entries.
class StatusIterator {
 public:
  /// @brief The iterator category. Required by the C++ standard library.
  using iterator_category = std::random_access_iterator_tag;

  /// @brief The type of the object that the iterator points to.
  using ValueType = StatusEntry;

  /// @brief A type that can represent the distance between two iterators.
  using DifferenceType = std::size_t;

  /// @brief The type of a pointer to the value type (`StatusEntry*`).
  using Pointer = ValueType*;

  /// @brief The type of a reference to the value type (`StatusEntry&`).
  using ReferenceType = ValueType&;

  /// @brief Pre-increment operator.
  StatusIterator& operator++() noexcept;

  /// @brief Post-increment operator.
  StatusIterator operator++(int) noexcept;

  /// @brief Pre-increment operator.
  StatusIterator& operator--() noexcept;

  /// @brief Post-increment operator.
  StatusIterator operator--(int) noexcept;

  /// @brief Subscript operator.
  ReferenceType operator[](DifferenceType n) const noexcept;

  /// @brief Addition operator.
  StatusIterator operator+(DifferenceType n) noexcept;

  /// @brief Assignment addition operator.
  StatusIterator& operator+=(DifferenceType n) noexcept;

  /// @brief Subtraction operator.
  StatusIterator operator-(DifferenceType n) noexcept;

  /// @brief Assignment subtraction operator.
  StatusIterator& operator-=(DifferenceType n) noexcept;

  /// @brief Dereference operator.
  ReferenceType operator*() noexcept;

  /// @brief Addition operator.
  friend DifferenceType operator+(const StatusIterator& lhs,
                                  const StatusIterator& rhs) noexcept;

  /// @brief Subtraction operator.
  friend DifferenceType operator-(const StatusIterator& lhs,
                                  const StatusIterator& rhs) noexcept;

  /// @brief Spaceship operator.
  friend std::strong_ordering operator<=>(const StatusIterator& lhs,
                                         const StatusIterator& rhs) noexcept;

  /// @brief Equality operator.
  // shouldn't the spaceship define this???
  friend bool operator==(const StatusIterator& lhs,
                         const StatusIterator& rhs) noexcept;

  friend class Status;

 private:
  /// @brief Private constructor.
  explicit StatusIterator(const Status& status);

  /// @brief Updates the status entry.
  void updateStatusEntry() noexcept;

  /// @brief The index of the current status entry.
  std::size_t m_index;

  /// @brief The status list.
  std::shared_ptr<git_status_list> m_statusList;

  /// @brief The current status entry.
  StatusEntry m_statusEntry;

  /// @brief The number of status entries.
  std::size_t m_statusCount;
};

/// @brief Subtraction operator.
[[nodiscard]] StatusIterator::DifferenceType operator-(
    const StatusIterator& lhs, const StatusIterator& rhs) noexcept;

/// @brief Spaceship operator.
[[nodiscard]] std::strong_ordering operator<=>(const StatusIterator& lhs,
                                              const StatusIterator& rhs) noexcept;

/// @brief Equality operator.
// shouldn't the spaceship define this???
[[nodiscard]] bool operator==(const StatusIterator& lhs,
                              const StatusIterator& rhs) noexcept;

}  // namespace gitxx
#endif  // INCLUDE_GITXX_STATUS_HPP_
