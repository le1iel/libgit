#include <gtest/gtest.h>
#include <filesystem>

#include <libgit/init.hpp>
#include <libgit/repository.hpp>

#include "../../common/git_commands.h"

class repository_ut : public ::testing::Test {
 public:
  const std::string repo_path { std::filesystem::absolute(std::filesystem::path("/tmp/bt_repository")).string() };

  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

TEST_F(repository_ut, open_non_repo) {
  EXPECT_FALSE(git::Repository::Open(repo_path));
}

TEST_F(repository_ut, open_success) {
  git::GitCommands repo{repo_path};
  EXPECT_TRUE(git::Repository::Open(repo_path));
}

TEST_F(repository_ut, head) {
  git::GitCommands repo{repo_path};
  repo.makeEmptyCommit("1");

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(repoRes->head().has_value());
}

TEST_F(repository_ut, path) {
  git::GitCommands repo{repo_path};
  repo.makeEmptyCommit("1");

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  std::string repo_path_str = repoRes->path();
  // TODO fix this test for all platforms
  // (macos make /tmp into /private/tmp)
  // EXPECT_EQ(repoRes->path(), repo_path + "/.git/");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
