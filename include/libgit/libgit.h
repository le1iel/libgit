
#include <libgit/repository.h>
#include <optional>
#include <string_view>

namespace git {

std::optional<Repository> OpenRepository(std::string_view path) noexcept;

}
