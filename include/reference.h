#ifndef GIT_REFERENCE_H
#define GIT_REFERENCE_H
#include <git2/errors.h>
#include <git2/refs.h>
#include <memory>
#include <string>

class Reference {

public:

  Reference(git_reference *ptr);

  std::string name() const noexcept;
  Reference(const Reference &other);

  bool is_branch() const noexcept;
  bool is_tag() const noexcept;
  bool is_remote() const noexcept;
  bool is_note() const noexcept;

  std::string shorthand() const noexcept;

  std::optional<git_error> resolve() noexcept;

  git_reference_t type() const noexcept;

  git_object* peel() const noexcept;

private:
  struct GitRefDeletor {
    void operator()(git_reference *ref) { git_reference_free(ref); };
  };

  std::unique_ptr<git_reference, GitRefDeletor> m_ref;
};

#endif
