#ifndef INCLUDE_GITXX_STATUS_HPP_
#define INCLUDE_GITXX_STATUS_HPP_

#include <flagfield.hpp>
#include <gitxx/diff_delta.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status_options.hpp>
#include <memory>
#include <optional>

// forward declaration to hide libgit2 headers
struct git_status_list;

namespace gitxx {

/// @brief Enum representing the status of a file in the repository.
enum class FileStatus {
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

struct StatusEntry {
  gitxx::FlagField<gitxx::FileStatus> status{0};
  std::optional<DiffDelta> head_to_index;
  std::optional<DiffDelta> index_to_workdir;
};

// forward declaration
class StatusIterator;

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

class StatusIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using ValueType = StatusEntry;
  using DifferenceType = std::size_t;
  using Pointer = ValueType*;
  using ReferenceType = ValueType&;

  /// @brief Move constructor.
  StatusIterator(StatusIterator&&) noexcept = default;

  /// @brief Copy constructor.
  StatusIterator(const StatusIterator&) noexcept = default;

  /// @brief Move assignment operator.
  StatusIterator& operator=(StatusIterator&&) noexcept = default;

  /// @brief Copy assignment operator.
  StatusIterator& operator=(const StatusIterator&) noexcept = default;

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
  friend DifferenceType operator+(StatusIterator lhs,
                                  StatusIterator rhs) noexcept;

  /// @brief Subtraction operator.
  friend DifferenceType operator-(const StatusIterator lhs,
                                  const StatusIterator rhs) noexcept;

  /// @brief Spaceship operator.
  friend auto operator<=>(const StatusIterator& lhs,
                          const StatusIterator& rhs) noexcept;

  /// @brief Equality operator.
  // shouldn't the spaceship define this???
  friend bool operator==(const StatusIterator& lhs,
                         const StatusIterator& rhs) noexcept;

  friend class Status;

 private:
  /// @brief Private constructor.
  StatusIterator(const Status& status);

  /// @brief Updates the status entry.
  void updateStatusEntry() noexcept;

  /// @brief The index of the current status entry.
  std::size_t m_index;

  /// @brief The number of status entries.
  std::size_t m_statusCount;

  /// @brief The current status entry.
  StatusEntry m_statusEntry;

  /// @brief The status list.
  std::shared_ptr<git_status_list> m_statusList;
};

/// @brief Subtraction operator.
[[nodiscard]] StatusIterator::DifferenceType operator-(
    const StatusIterator lhs, const StatusIterator rhs) noexcept;

/// @brief Spaceship operator.
[[nodiscard]] auto operator<=>(const StatusIterator& lhs,
                               const StatusIterator& rhs) noexcept;

/// @brief Equality operator.
// shouldn't the spaceship define this???
[[nodiscard]] bool operator==(const StatusIterator& lhs,
                              const StatusIterator& rhs) noexcept;

}  // namespace gitxx
#endif  // INCLUDE_GITXX_STATUS_HPP_
