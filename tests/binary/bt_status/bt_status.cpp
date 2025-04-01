#include <gtest/gtest.h>
#include <libgit/repository.h>
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

TEST_F(status_ut, new_file) {
  git::GitCommands repo{repo_path};
  repo.createFile("test.txt", "content");
  
  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::StatusOptions options{};
  git::StatusIterator it{&repoRes.value(), options};
  
//   EXPECT_EQ((*it).status, git::FileStatus::WtName);
}

TEST_F(status_ut, modified_file) {
  git::GitCommands repo{repo_path};
  repo.createFile("test.txt", "content");
  repo.add("test.txt");
  repo.commit("Initial commit");
  repo.modifyFile("test.txt", "new content");
  
  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::StatusOptions options{};
  git::StatusIterator it{&repoRes.value(), options};
  
//   EXPECT_EQ(it.operator*().status, git::FileStatus::WtModified);
}

TEST_F(status_ut, deleted_file) {
  git::GitCommands repo{repo_path};
  repo.createFile("test.txt", "content");
  repo.add("test.txt");
  repo.commit("Initial commit");
  repo.deleteFile("test.txt");
  
  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::StatusOptions options{};
  git::StatusIterator it{&repoRes.value(), options};
  
    EXPECT_EQ(it.operator*().status.test(static_cast<std::size_t>(git::FileStatus::WtDeleted)), true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}