#ifndef INCLUDE_GITXX_REFERENCE_HPP_
#define INCLUDE_GITXX_REFERENCE_HPP_
#include <cstdint>
#include <expected>
#include <memory>
#include <string>

#include <gitxx/error.hpp>

// forward declaration to hide libgit2 headers
struct git_reference;

namespace gitxx {

/// @brief Basic type of any Git reference.
enum class ReferenceType : std::uint8_t {
  /// @brief Invalid reference.
  Invalid = 0,
  /// @brief A reference that points at an object id.
  Direct,
  /// @brief A reference that points at another reference.
  Symbolic,
  /// @brief Both direct and symbolic.
  All,
};

/// @brief References point to a commit; generally these are branches and tags.
class Reference {
 public:
  /// @brief Constructorable from the libgit2 object.
  // will probably make this private later
  explicit Reference(git_reference* ptr);

  /// @brief Move constructable.
  Reference(Reference&& other) = default;

  /// @brief Move assignable.
  Reference& operator=(Reference&& other) = default;

  /// @brief Copy constructable.
  Reference(const Reference& other) = default;

  /// @brief Copy assignable.
  Reference& operator=(const Reference& other) = default;

  /// @brief Destructor.
  ~Reference() = default;

  /// @brief Returns the name of the reference.
  [[nodiscard]] std::string name() const noexcept;

  /// @brief Returns true if the reference is a branch.
  [[nodiscard]] bool isBranch() const noexcept;

  /// @brief Returns true if the reference is a tag.
  [[nodiscard]] bool isTag() const noexcept;

  /// @brief Returns true if the reference is a note.
  [[nodiscard]] bool isNote() const noexcept;

  /// @brief Returns true if the reference is a remote.
  [[nodiscard]] bool isRemote() const noexcept;

  /// @brief Returns the shorthand of the reference.
  [[nodiscard]] std::string shorthand() const noexcept;

  /// @brief Resolves the reference.
  [[nodiscard]] std::expected<void, GitErrc> resolve() noexcept;

  /// @brief Returns the type of the reference.
  [[nodiscard]] ReferenceType type() const noexcept;

 private:
  /// @brief The libgit2 reference object.
  std::shared_ptr<git_reference> m_ref;
};

}  // namespace gitxx
#endif  // INCLUDE_GITXX_REFERENCE_HPP_
