#ifndef INCLUDE_GITXX_DIFF_FILE_HPP_
#define INCLUDE_GITXX_DIFF_FILE_HPP_

#include <cstdint>
#include <filesystem>
#include <gitxx/object_id.hpp>

namespace git {

struct DiffFile {
  ObjectId old_id;
  std::filesystem::path path;
  std::uint64_t size;
  std::uint32_t flags;
  std::uint32_t mode;
  std::uint16_t id_abbrev;
};

}  // namespace git

#endif  // INCLUDE_GITXX_DIFF_FILE_HPP_
