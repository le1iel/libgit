#include <git_commands.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <gitxx/init.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>

class status_ut : public ::testing::Test {
 public:
  const std::string repo_path{
      std::filesystem::absolute(std::filesystem::path("/tmp/bt_status"))
          .string()};

  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

/// @brief Test that a new file is not in the status list.
/// @note The status will not show new files it the option is not set.
TEST_F(status_ut, new_file) {
  git::GitCommands internalRepo{repo_path};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());
  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status.none());
}

TEST_F(status_ut, modified_file) {
  git::GitCommands internalRepo{repo_path};

  std::string modified_file = "test.txt";

  ASSERT_TRUE(internalRepo.createFile(modified_file, "content"));
  ASSERT_TRUE(internalRepo.add(modified_file));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));
  ASSERT_TRUE(internalRepo.modifyFile(modified_file, "new content"));

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());
  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status[git::FileStatus::WtModified]);
}

TEST_F(status_ut, deleted_file) {
  git::GitCommands internalRepo{repo_path};
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));
  ASSERT_TRUE(internalRepo.deleteFile("test.txt"));

  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());
  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status[git::FileStatus::WtDeleted]);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
