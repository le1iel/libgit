#ifndef GITXX_TESTS_MOCKS_LIBGIT2_MOCK_REPOSITORY_H
#define GITXX_TESTS_MOCKS_LIBGIT2_MOCK_REPOSITORY_H

#include <gmock/gmock.h>
#include <libgit2/repository.h>

namespace libgit {
namespace mocks {

class MockRepository {
 public:
  MockRepository();

  MOCK_METHOD(int, repository_open, (git_repository * *repo, const char *path),
              ());
  MOCK_METHOD(void, repository_free, (git_repository * repo), ());
  MOCK_METHOD(int, repository_init,
              (git_repository * *repo, const char *path, unsigned is_bare), ());
  MOCK_METHOD(int, repository_clone,
              (git_repository * *repo, const char *url, const char *local_path,
               const git_clone_options *options),
              ());
};

std::unique_ptr<MockRepository> g_mockRepository = nullptr;

}  // namespace mocks
}  // namespace libgit

// C function implementations that delegate to the mock
extern "C" {
int git_repository_open(git_repository **repo, const char *path) {
  return libgit::mocks::g_mockRepository->repository_open(repo, path);
}

void git_repository_free(git_repository *repo) {
  libgit::mocks::g_mockRepository->repository_free(repo);
}

int git_repository_init(git_repository **repo, const char *path,
                        unsigned is_bare) {
  return libgit::mocks::g_mockRepository->repository_init(repo, path, is_bare);
}

int git_repository_clone(git_repository **repo, const char *url,
                         const char *local_path,
                         const git_clone_options *options) {
  return libgit::mocks::g_mockRepository->repository_clone(repo, url,
                                                           local_path, options);
}
}

#endif  // GITXX_TESTS_MOCKS_LIBGIT2_MOCK_REPOSITORY_H
