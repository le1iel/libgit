#include <gtest/gtest.h>

#include <filesystem>
#include <gitxx/repository.hpp>

#include "git_commands.hpp"

class bt_repository : public ::testing::Test {};

TEST_F(bt_repository, open_non_repo) {
  std::filesystem::path repo_path("/tmp/random_dir");
  std::filesystem::create_directories(repo_path);
  EXPECT_FALSE(gitxx::Repository::Open(repo_path));
  std::filesystem::remove(repo_path);
}

TEST_F(bt_repository, open_success) {
  gitxx::GitCommands internalRepo{};
  EXPECT_TRUE(gitxx::Repository::Open(internalRepo.path()));
}

TEST_F(bt_repository, head) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(repoRes->head().has_value());
}

TEST_F(bt_repository, path) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_TRUE(
      std::filesystem::equivalent(repoRes->path(), (repo.path() / ".git")));
}

TEST_F(bt_repository, open_error_code) {
  std::filesystem::path repo_path("/tmp/random_dir_for_error_test");
  std::filesystem::create_directories(repo_path);

  auto res = gitxx::Repository::Open(repo_path);

  ASSERT_FALSE(res.has_value());
  EXPECT_EQ(res.error(), GitErrc::not_found);

  std::filesystem::remove_all(repo_path);
}

TEST_F(bt_repository, head_unborn) {
  gitxx::GitCommands repo{};

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  EXPECT_FALSE(repoRes->head().has_value());
}

TEST_F(bt_repository, head_name) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());
  EXPECT_EQ(head->name(), "refs/heads/main");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
