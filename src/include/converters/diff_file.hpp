#ifndef SRC_INCLUDE_CONVERTERS_DIFF_FILE_HPP_
#define SRC_INCLUDE_CONVERTERS_DIFF_FILE_HPP_

#include <git2/diff.h>

#include <converters/base.hpp>
#include <gitxx/diff_file.hpp>
#include <gitxx/diff_flag.hpp>
#include <string>
#include <flagfield.hpp>

namespace gitxx::internal {

template <>
struct conversion_traits<gitxx::DiffFile, git_diff_file> {
  static gitxx::DiffFile from_c(const git_diff_file& file) {
    return gitxx::DiffFile{
        .old_id{},
        .path{std::string(file.path)},
        .size = file.size,
        .flags = gitxx::FlagField<gitxx::DiffFlag>{0U},
        .mode = file.mode,
    };
  }
};

};  // namespace gitxx::internal

#endif  // SRC_INCLUDE_CONVERTERS_DIFF_FILE_HPP_
