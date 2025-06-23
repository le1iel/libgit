#ifndef INCLUDE_GITXX_REFERENCE_HPP_
#define INCLUDE_GITXX_REFERENCE_HPP_
#include <memory>
#include <string>

// forward declaration to hide libgit2 headers
struct git_reference;

namespace gitxx {

enum class ReferenceType {
  Invalid = 0,
  Direct,
  Symbolic,
};

class Reference {
 public:
  // will probably make this private later
  Reference(git_reference *ptr);

  /// @brief Move constructable. 
  Reference(Reference &&other) = default;

  /// @brief Move assignable. 
  Reference &operator=(Reference &&other) = default;

  /// @brief Copy constructable. 
  Reference(const Reference &other) = default;

  /// @brief Copy assignable. 
  Reference &operator=(const Reference &other) = default;

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
  [[nodiscard]] int resolve() noexcept;

  /// @brief Returns the type of the reference.
  [[nodiscard]] ReferenceType type() const noexcept;

 private:

  /// @brief The libgit2 reference object.
  std::shared_ptr<git_reference> m_ref;
};

}  // namespace gitxx
#endif  // INCLUDE_GITXX_REFERENCE_HPP_
