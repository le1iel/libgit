#include <gtest/gtest.h>
#include <filesystem>

#include <libgit/repository.hpp>
#include <libgit/status_options.hpp>
#include <libgit/init.hpp>
#include <flagfield.hpp>
#include <libgit/status.hpp>

#include "../../common/git_commands.h"

class status_ut : public ::testing::Test {
 public:
  const std::string repo_path { std::filesystem::absolute(std::filesystem::path("/tmp/bt_status")).string() };

  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

// Updated new_file_untracked test case
TEST_F(status_ut, new_file_untracked) {
  git::GitCommands internalRepo{repo_path};
  internalRepo.makeEmptyCommit("one");
  internalRepo.createFile("test.txt", "content");
  internalRepo.createFile("test2.txt", "content"); // Creating another file for additional cases
  internalRepo.printStatus();

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status.none()); // Check for no untracked files
}

// Updated 2_new_file_untracked test case
TEST_F(status_ut, 2_new_file_untracked) {
  git::GitCommands internalRepo{repo_path};
  internalRepo.makeEmptyCommit("one");
  internalRepo.createFile("test.txt", "content");
  internalRepo.createFile("test2.txt", "content");
  internalRepo.printStatus();

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status(git::StatusOptions {
    .show = git::StatusShow::IndexAndWorkdir,
    .flags = git::FlagField<git::StatusFlags> {static_cast<std::uint32_t>(git::StatusFlags::IncludeUntracked) + 1},
  });
  auto status = std::move(statusRes).value();

  auto it = status.begin();
  EXPECT_TRUE(it.operator*().status.any()); // Check first file is untracked
  ++it;
  EXPECT_TRUE(it.operator*().status.any()); // Check second file is also untracked
}

// Updated modified_file test case
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

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status[git::FileStatus::WtModified]); // Check for modified status
}

// Updated deleted_file test case
TEST_F(status_ut, deleted_file) {
  git::GitCommands internalRepo{repo_path};
  internalRepo.createFile("test.txt", "content");
  internalRepo.add("test.txt");
  internalRepo.commit("Initial commit");

  internalRepo.deleteFile("test.txt");

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status[git::FileStatus::WtDeleted]); // Check for deleted status
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
