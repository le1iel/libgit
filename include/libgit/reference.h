#ifndef GIT_REFERENCE_H
#define GIT_REFERENCE_H
#include <memory>
#include <string>

// forward declaration to hide libgit2 headers
struct git_reference;

namespace git {

enum class ReferenceType {
  Invalid = 0,
  Direct,
  Symbolic,
};

class Reference {

public:
  Reference(git_reference *ptr);

  /// @brief Returns the name of the reference.
  std::string name() const noexcept;

  /// @brief Copy constructor.
  Reference(const Reference &other);

  /// @brief Destructor.
  ~Reference() = default;

  /// @brief Returns true if the reference is a branch.
  bool isBranch() const noexcept;

  /// @brief Returns true if the reference is a tag.
  bool isTag() const noexcept;

  /// @brief Returns true if the reference is a note.
  bool isNote() const noexcept;

  /// @brief Returns true if the reference is a remote.
  bool isRemote() const noexcept;

  /// @brief Returns the shorthand of the reference.
  std::string shorthand() const noexcept;

  /// @brief Resolves the reference.
  int resolve() noexcept;

  /// @brief Returns the type of the reference.
  ReferenceType type() const noexcept;

private:
  /// @brief Deleter for the reference.
  struct GitReferenceDeletor {
    void operator()(git_reference *ref) const noexcept;
  };

  std::unique_ptr<git_reference, GitReferenceDeletor> m_ref;
};

} // namespace git
#endif
