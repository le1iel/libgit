#include <libgit/repository.h>
#include <libgit/status.h>

namespace git {

StatusIter::StatusIter(Repository &repo) noexcept { repo.m_repo.get(); }

} // namespace git
