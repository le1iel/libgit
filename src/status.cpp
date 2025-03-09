#include <libgit/status.h>
#include <libgit/repository.h>


namespace git
{

StatusIter::StatusIter(Repository& repo) noexcept
{
    repo.m_repo.get();
}

}
