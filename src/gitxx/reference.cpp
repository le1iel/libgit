#include <git2/refs.h>
#include <git2/types.h>

#include <gitxx/reference.hpp>
#include <memory>

namespace gitxx {

Reference::Reference(git_reference *ptr)
    : m_ref(std::shared_ptr<git_reference>(ptr, git_reference_free)) {};

std::string Reference::name() const noexcept {
  std::string name{};
  name = git_reference_name(m_ref.get());
  return name;
}

bool Reference::isBranch() const noexcept {
  return git_reference_is_branch(m_ref.get()) != 0;
  return false;
}

bool Reference::isNote() const noexcept {
  return git_reference_is_note(m_ref.get()) != 0;
  return false;
}

bool Reference::isTag() const noexcept {
  return git_reference_is_tag(m_ref.get()) != 0;
  return false;
}

bool Reference::isRemote() const noexcept {
  return git_reference_is_remote(m_ref.get()) != 0;
  return false;
}

std::string Reference::shorthand() const noexcept {
  std::string out{};
  out = git_reference_shorthand(m_ref.get());
  return out;
}

int Reference::resolve() noexcept {
  git_reference *ref = nullptr;
  int res = git_reference_resolve(&ref, m_ref.get());
  if (res < 0) {
    return -1;
  }

  m_ref.reset(ref, git_reference_free);
  return 0;
}

}  // namespace gitxx
