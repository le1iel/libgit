#include <git2/refs.h>
#include <git2/types.h>

#include <converters/error.hpp>
#include <gitxx/reference.hpp>
#include <memory>
#include <string>

namespace gitxx {

Reference::Reference(git_reference* ptr)
    : m_ref(std::shared_ptr<git_reference>(ptr, git_reference_free)) {};

std::string Reference::name() const noexcept {
  std::string name{};
  name = git_reference_name(m_ref.get());
  return name;
}

bool Reference::isBranch() const noexcept {
  return git_reference_is_branch(m_ref.get()) != 0;
}

bool Reference::isNote() const noexcept {
  return git_reference_is_note(m_ref.get()) != 0;
}

bool Reference::isTag() const noexcept {
  return git_reference_is_tag(m_ref.get()) != 0;
}

bool Reference::isRemote() const noexcept {
  return git_reference_is_remote(m_ref.get()) != 0;
}

std::string Reference::shorthand() const noexcept {
  return git_reference_shorthand(m_ref.get());
}

ReferenceType Reference::type() const noexcept {
  return static_cast<ReferenceType>(git_reference_type(m_ref.get()));
}

std::expected<void, GitErrc> Reference::resolve() noexcept {
  git_reference* ref = nullptr;
  const int res = git_reference_resolve(&ref, m_ref.get());
  if (res < 0) {
    return std::unexpected<GitErrc>(
        internal::conversion_traits<GitErrc, int>::from_c(res));
  }

  m_ref.reset(ref, git_reference_free);
  return {};
}

}  // namespace gitxx
