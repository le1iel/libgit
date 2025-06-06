#include <cstdlib>
#include <libgit/error.hpp>

namespace git {

constexpr GitErrc Error(int internal_error) noexcept {
  return static_cast<GitErrc>(std::abs(internal_error));
}

}  // namespace git
