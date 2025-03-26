#include <git2/refs.h>
#include <git2/types.h>
#include <libgit/reference.h>
#include <memory>
#include <optional>

namespace git {

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

bool Reference::isBranch() const noexcept {
  return git_reference_is_branch(m_ref.get());
  return false;
}

bool Reference::isNote() const noexcept {
  return git_reference_is_note(m_ref.get());
  return false;
}

bool Reference::isTag() const noexcept {
  return git_reference_is_tag(m_ref.get());
  return false;
}

bool Reference::isRemote() const noexcept {
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

std::optional<git_error> Reference::resolve() noexcept {
  git_reference *ref = nullptr;
  int res = git_reference_resolve(&ref, m_ref.get());
  if (res < 0) {
    return *git_error_last();
  }

  m_ref.reset(ref);
  return std::nullopt;
}

} // namespace git
