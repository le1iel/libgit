#ifndef SRC_INCLUDE_CONVERTERS_STATUS_OPTIONS_HPP_
#define SRC_INCLUDE_CONVERTERS_STATUS_OPTIONS_HPP_

#include <git2/status.h>

#include <converters/base.hpp>
#include <gitxx/status_options.hpp>

namespace gitxx::internal {

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

};  // namespace gitxx::internal

#endif  // SRC_INCLUDE_CONVERTERS_STATUS_OPTIONS_HPP_
