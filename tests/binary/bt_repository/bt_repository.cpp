#include <gtest/gtest.h>
#include <libgit/repository.h>
#include <libgit/init.h>

#include "../../common/git_commands.h"

class repository_ut : public ::testing::Test {
 public:
  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

TEST_F(repository_ut, open_non_repo) {
  std::string repo_path{"/tmp/not_a_repo"};
  EXPECT_FALSE(git::Repository::Open(repo_path));
}

TEST_F(repository_ut, open_success) {
  git::GitCommands repo{"/tmp/something"};
  EXPECT_TRUE(git::Repository::Open(repo.path()));
}

TEST_F(repository_ut, head) {
  git::GitCommands repo{"/tmp/something"};
  repo.makeEmptyCommit("1");

  auto repoRes = git::Repository::Open("/tmp/something");
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(repoRes->head().has_value());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
