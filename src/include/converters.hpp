#include <git2/status.h>

#include <cstdlib>
#include <gitxx/diff_delta.hpp>
#include <gitxx/error.hpp>
#include <gitxx/status.hpp>

#include "gitxx/status_options.hpp"

namespace gitxx::internal {

template <typename CppType, typename CType>
struct conversion_traits;

// TODO How to move these to the cpp files
template <>
struct conversion_traits<GitErrc, int> {
  static GitErrc from_c(const int& error) {
    return static_cast<GitErrc>(std::abs(error));
  }

  static int from_cpp(const GitErrc& error) { return static_cast<int>(error); }
};

template <>
struct conversion_traits<git_status_options, gitxx::StatusOptions> {
  static std::optional<gitxx::StatusOptions> from_c(
      const git_status_options* option) {
      // do we ever need this?
      static_cast<void>(option);
      return std::nullopt;
  }

  static git_status_options from_cpp(const gitxx::StatusOptions& options) {
    return {.version = 1U,
            .show = static_cast<git_status_show_t>(options.show),
            .flags = options.flags.value(),
            .pathspec = {nullptr, 0U},
            .baseline = nullptr,
            .rename_threshold = options.rename_threshold};
  }
};



}  // namespace gitxx::internal
