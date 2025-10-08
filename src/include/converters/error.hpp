#ifndef SRC_INCLUDE_CONVERTERS_ERROR_HPP_
#define SRC_INCLUDE_CONVERTERS_ERROR_HPP_

#include <converters/base.hpp>
#include <gitxx/error.hpp>

namespace gitxx::internal {

template <>
struct conversion_traits<GitErrc, int> {
  static GitErrc from_c(const int& error) {
    return static_cast<GitErrc>(std::abs(error));
  }

  static int from_cpp(const GitErrc& error) { return static_cast<int>(error); }
};

};  // namespace gitxx::internal

#endif  // SRC_INCLUDE_CONVERTERS_ERROR_HPP_
