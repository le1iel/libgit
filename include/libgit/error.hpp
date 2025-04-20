#ifndef INCLUDE_LIBGIT_ERROR_H_
#define INCLUDE_LIBGIT_ERROR_H_

#include <system_error>

enum class GitErrc
{
    example = 1,
};

struct GitErrcCategory: std::error_category
{
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};

namespace std
{
  template <>
    struct is_error_code_enum<GitErrc> : true_type {};
}

std::error_code make_error_code(GitErrc);

const GitErrcCategory gitErrcCategory {};

#endif  // INCLUDE_LIBGIT_ERROR_H_
