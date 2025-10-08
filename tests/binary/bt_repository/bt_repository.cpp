#include <gtest/gtest.h>

#include <filesystem>
#include <gitxx/init.hpp>
#include <gitxx/repository.hpp>

#include "git_commands.h"

class repository_ut : public ::testing::Test {
 public:
  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

TEST_F(repository_ut, open_non_repo) {
  std::filesystem::path repo_path("/tmp/random_dir");
  std::filesystem::create_directories(repo_path);
  EXPECT_FALSE(gitxx::Repository::Open(repo_path));
  std::filesystem::remove(repo_path);
}

TEST_F(repository_ut, open_success) {
  gitxx::GitCommands internalRepo{};
  EXPECT_TRUE(gitxx::Repository::Open(internalRepo.path()));
}

TEST_F(repository_ut, head) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(repoRes->head().has_value());
}

TEST_F(repository_ut, path) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(
      std::filesystem::equivalent(repoRes->path(), (repo.path() / ".git")));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
