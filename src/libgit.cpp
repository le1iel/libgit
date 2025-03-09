#include "repository_impl.h"
#include <libgit/libgit.h>
#include <libgit/repository.h>

namespace git
{

std::optional<git::Repository>
OpenRepository (std::string_view path) noexcept
{
    RepositoryImpl r{ path };
}

}; // namespace git
