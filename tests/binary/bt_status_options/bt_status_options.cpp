#include <gtest/gtest.h>
#include <libgit/repository.h>
#include <libgit/status_options.h>
#include <libgit/init.h>
#include <libgit/status.h>
#include <filesystem>

#include "../../common/git_commands.h"

class status_ut : public ::testing::Test {
 public:
  const std::string repo_path { std::filesystem::absolute(std::filesystem::path("/tmp/bt_status")).string() };

  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

/// @brief Test that a new file is not in the status list.
/// @note The status will not show new files it the option is not set.
TEST_F(status_ut, new_file) {
  git::GitCommands internalRepo{repo_path};
  internalRepo.makeEmptyCommit("one");
  internalRepo.createFile("test.txt", "content");
  internalRepo.printStatus();

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());
  git::Repository repo = std::move(repoRes.value());

  auto it = repo.status();

  EXPECT_TRUE(it.operator*().status.none());
}

TEST_F(status_ut, modified_file) {
  git::GitCommands internalRepo{repo_path};

  std::string modified_file = "test.txt";

  internalRepo.createFile(modified_file, "content");
  internalRepo.add(modified_file);
  internalRepo.commit("Initial commit");
  internalRepo.modifyFile(modified_file, "new content");

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());
  git::Repository repo = std::move(repoRes.value());

  auto it = repo.status();

  EXPECT_TRUE(it.operator*().status[git::FileStatus::WtModified]);
}

TEST_F(status_ut, deleted_file) {
  git::GitCommands internalRepo{repo_path};
  internalRepo.createFile("test.txt", "content");
  internalRepo.add("test.txt");
  internalRepo.commit("Initial commit");

  internalRepo.deleteFile("test.txt");

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());
  git::Repository repo = std::move(repoRes.value());

  auto it = repo.status();

  EXPECT_TRUE(it.operator*().status[git::FileStatus::WtDeleted]);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}