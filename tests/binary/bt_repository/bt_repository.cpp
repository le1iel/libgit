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
  EXPECT_FALSE(git::Repository::Open(repo_path));
  std::filesystem::remove(repo_path);
}

TEST_F(repository_ut, open_success) {
  git::GitCommands internalRepo{};
  EXPECT_TRUE(git::Repository::Open(internalRepo.path()));
}

TEST_F(repository_ut, head) {
  git::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = git::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(repoRes->head().has_value());
}

TEST_F(repository_ut, path) {
  git::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = git::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  std::string repo_path_str = repoRes->path();
  EXPECT_TRUE(
      std::filesystem::equivalent(repoRes->path(), (repo.path() / ".git")));
  std::cout << repo.path() << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
