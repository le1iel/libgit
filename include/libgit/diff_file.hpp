#ifndef INCLUDE_LIBGIT_DIFF_FILE_H_
#define INCLUDE_LIBGIT_DIFF_FILE_H_

#include <libgit/object_id.hpp>
#include <cstdint>
#include <string>

namespace git {

struct DiffFile {
    ObjectId old_id;
    std::string path;
    std::uint64_t size;
    std::uint32_t flags;
    std::uint32_t mode;
    std::uint16_t id_abbrev;
};

} // namespace git

#endif  // INCLUDE_LIBGIT_DIFF_FILE_H_

