#include <cstdlib>
#include <gitxx/diff_delta.hpp>
#include <gitxx/error.hpp>

namespace git {

namespace internal {

constexpr GitErrc Error(int internal_error) noexcept {
  return static_cast<GitErrc>(std::abs(internal_error));
}

// constexpr DiffDelta diffDelta(int internal_error) noexcept {
//   return static_cast<GitErrc>(std::abs(internal_error));
// }

}  // namespace internal

}  // namespace git
