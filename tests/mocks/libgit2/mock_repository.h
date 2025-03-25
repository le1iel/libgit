#include <gmock/gmock.h>
#include <libgit2/repository.h>

namespace libgit {
namespace mocks {
class MockRepositry {
  MOCK_METHOD(int, repository_open, (git_repository **, char *));
}
std::unique_ptr<MockRepositry> s_mockRepository = nullptr;

} // namespace mocks
} // namespace libgit

git_repository_open(git_repository **repo, char *path) {
    libgit::mocks::s_mockRepository->repository_open(repo, path);
}
