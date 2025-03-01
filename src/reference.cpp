#include <reference.h>
#include <git2/refs.h>
#include <git2/types.h>
#include <memory>
#include <optional>

Reference::Reference(git_reference *ptr)
    : m_ref(std::unique_ptr<git_reference, GitRefDeletor>(ptr)) {};

Reference::Reference(const Reference &other) {
  git_reference *ref = nullptr;
  git_reference_dup(&ref, other.m_ref.get());
  m_ref = std::unique_ptr<git_reference, GitRefDeletor>(ref);
}

std::string Reference::name() const noexcept {
  std::string name{};
  name = git_reference_name(m_ref.get());
  return name;
}

bool Reference::is_branch() const noexcept {
  return git_reference_is_branch(m_ref.get());
  return false;
}

bool Reference::is_note() const noexcept {
  return git_reference_is_note(m_ref.get());
  return false;
}

bool Reference::is_tag() const noexcept {
  return git_reference_is_tag(m_ref.get());
  return false;
}

bool Reference::is_remote() const noexcept {
  return git_reference_is_remote(m_ref.get());
  return false;
}

std::string Reference::shorthand() const noexcept {
  std::string out{};
  out = git_reference_shorthand(m_ref.get());
  return out;
}

git_reference_t Reference::type() const noexcept {
  return git_reference_type(m_ref.get());
}

git_object *Reference::peel() const noexcept {
  git_object *out = nullptr;
  git_reference_peel(&out, m_ref.get(), GIT_OBJECT_ANY);
  return out;
}

std::optional<git_error> Reference::resolve() noexcept {
  git_reference *ref = nullptr;
  int res = git_reference_resolve(&ref, m_ref.get());
  if (res < 0) {
    return *git_error_last();
  }

  m_ref.reset(ref);
  return std::nullopt;
}
