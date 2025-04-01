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
  repo.makeEmptyCommit("one");
  repo.createFile("test.txt", "content");
  repo.printStatus();
  
  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::Repository grepo = std::move(repoRes.value());
  git::StatusOptions options{};

  auto it = grepo.status();
  
  EXPECT_TRUE(it.operator*().status.none());
}

TEST_F(status_ut, modified_file) {
  git::GitCommands repo{repo_path};

  std::string modified_file = "test.txt";

  repo.createFile(modified_file, "content");
  repo.add(modified_file);
  repo.commit("Initial commit");
  repo.modifyFile(modified_file, "new content");
  
  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::Repository grepo = std::move(repoRes.value());
  git::StatusOptions options{};

  auto it = grepo.status();
  
  EXPECT_TRUE(it.operator*().status[git::FileStatus::WtModified]);
  ASSERT_TRUE(it.operator*().index_to_workdir.has_value());
  EXPECT_EQ(it.operator*().index_to_workdir->new_file.path, modified_file);
}

TEST_F(status_ut, deleted_file) {
  git::GitCommands repo{repo_path};
  repo.createFile("test.txt", "content");
  repo.add("test.txt");
  repo.commit("Initial commit");
  repo.deleteFile("test.txt");
  repo.printStatus();
  
  auto repoRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(repoRes.has_value());

  git::Repository grepo = std::move(repoRes.value());
  git::StatusOptions options{};

  auto it = grepo.status();
  
  EXPECT_TRUE(it.operator*().status[git::FileStatus::WtDeleted]);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}