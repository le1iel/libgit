#include <git_commands.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <flagfield.hpp>
#include <gitxx/init.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>

class status_ut : public ::testing::Test {
 public:
  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

// Updated new_file_untracked test case
TEST_F(status_ut, new_file_untracked) {
  git::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));

  auto repoRes =
      git::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = repoRes.value();
  auto statusRes = repo.status(git::StatusOptions{
      .flags =
          git::FlagField<git::StatusFlags>{git::StatusFlags::IncludeUntracked},
  });
  ASSERT_TRUE(statusRes.has_value());
  auto status = statusRes.value();

  EXPECT_EQ(status.end() - status.begin(), 1);

  auto it = *(status.begin());
  EXPECT_TRUE(it.status.test(git::FileStatus::WtNew));
}

// Updated new_file_untracked test case
TEST_F(status_ut, new_file_untracked_index) {
  git::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.makeEmptyCommit("start"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.printStatus());
  ASSERT_TRUE(internalRepo.add("test.txt"));

  auto repoRes =
      git::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = repoRes.value();
  auto statusRes = repo.status(git::StatusOptions{
      .show = git::StatusShow::Index,
      .flags =
          git::FlagField<git::StatusFlags>{
              git::StatusFlags::IncludeUntracked, git::StatusFlags::UpdateIndex,
              git ::StatusFlags::RenamesHeadToIndex},
  });
  ASSERT_TRUE(statusRes.has_value());
  auto status = statusRes.value();

  EXPECT_EQ(status.end() - status.begin(), 1);

  auto it = *(status.begin());
  EXPECT_TRUE(it.status.test(git::FileStatus::Current))
      << it.status.to_string();
}

TEST_F(status_ut, 2_new_file_untracked) {
  // setup
  git::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.createFile("test2.txt", "content"));

  auto repoRes =
      git::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status(git::StatusOptions{
      .flags =
          git::FlagField<git::StatusFlags>{git::StatusFlags::IncludeUntracked},
  });
  ASSERT_TRUE(statusRes.has_value());

  auto status = statusRes.value();

  EXPECT_EQ(status.end() - status.begin(), 2);
}

// Updated modified_file test case
TEST_F(status_ut, modified_file) {
  git::GitCommands internalRepo{};

  std::string modified_file = "test.txt";

  ASSERT_TRUE(internalRepo.createFile(modified_file, "content"));
  ASSERT_TRUE(internalRepo.add(modified_file));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));
  ASSERT_TRUE(internalRepo.modifyFile(modified_file, "new content"));

  auto repoRes =
      git::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(
      status.begin()
          .operator*()
          .status[git::FileStatus::WtModified]);  // Check for modified status
}

// Updated deleted_file test case
TEST_F(status_ut, deleted_file) {
  git::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));

  ASSERT_TRUE(internalRepo.deleteFile("test.txt"));

  auto repoRes =
      git::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(
      status.begin()
          .operator*()
          .status[git::FileStatus::WtDeleted]);  // Check for deleted status
}

TEST_F(status_ut, renamed_file) {
  git::GitCommands internalRepo{};

  ASSERT_TRUE(internalRepo.createFile("old_name.txt", "content"));
  ASSERT_TRUE(internalRepo.add("old_name.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));

  ASSERT_TRUE(internalRepo.renameFile("old_name.txt", "new_name.txt"));

  auto repoRes =
      git::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status(git::StatusOptions{
      .show = git::StatusShow::IndexAndWorkdir,
      .flags =
          git::FlagField<git::StatusFlags>{
              git::StatusFlags::RenamesFromRewrites,
              git::StatusFlags::RenamesIndexToWorkdir,
              git::StatusFlags::RenamesHeadToIndex},
  });

  auto status = std::move(statusRes).value();

  EXPECT_EQ(status.end() - status.begin(), 1);

  auto it = status.begin();
  EXPECT_TRUE((*it).status[git::FileStatus::IndexRenamed])
      << (*it).status.to_string();
  // Check for renamed status
}

// TEST_F(status_ut, type_changed_file) {
//   git::GitCommands internalRepo{};

//   ASSERT_TRUE(internalRepo.createFile("test", "content"));
//   ASSERT_TRUE(internalRepo.add("test"));
//   ASSERT_TRUE(internalRepo.commit("Initial commit"));
//   ASSERT_TRUE(internalRepo.deleteFile("test"));
//   ASSERT_TRUE(internalRepo.createDir("test"));
//   ASSERT_TRUE(internalRepo.createFile("test/a", "content"));

//   auto repoRes = git::Repository::Open(internalRepo.path().string());
//   ASSERT_TRUE(repoRes.has_value());

//   git::Repository repo = std::move(repoRes.value());

//   auto statusRes = repo.status();
//   auto status = std::move(statusRes).value();

//   auto it = status.begin();
//   EXPECT_TRUE((*it).status[git::FileStatus::IndexTypeChanged])
//       << (*it).status.to_string();
//   internalRepo.printStatus();
//   // Check for type-changed status
// }

TEST_F(status_ut, ignored_file) {
  git::GitCommands internalRepo{};

  ASSERT_TRUE(internalRepo.createFile(".gitignore", "*.log"));
  ASSERT_TRUE(internalRepo.add(".gitignore"));
  ASSERT_TRUE(internalRepo.commit("Added gitignore"));

  ASSERT_TRUE(internalRepo.createFile("ignored.log", "log content"));

  auto repoRes = git::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status(git::StatusOptions{
      .show = git::StatusShow::IndexAndWorkdir,
      .flags =
          git::FlagField<git::StatusFlags>{git::StatusFlags::IncludeIgnored},
  });
  auto status = std::move(statusRes).value();

  auto it = status.begin();
  EXPECT_TRUE((*it).status[git::FileStatus::Ignored])
      << (*it).status.to_string();
  // Check for ignored status
}

TEST_F(status_ut, conflicting_file) {
  git::GitCommands internalRepo{};

  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));

  ASSERT_TRUE(internalRepo.createBranchAndCheckout("feature"));
  ASSERT_TRUE(internalRepo.modifyFile("test.txt", "feature branch content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Feature branch changes"));

  ASSERT_TRUE(internalRepo.checkoutBranch("main"));
  ASSERT_TRUE(internalRepo.modifyFile("test.txt", "main branch content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Main branch changes"));

  ASSERT_TRUE(
      internalRepo.mergeBranch("feature", true));  // Simulate a conflict

  auto repoRes = git::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  git::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  auto it = status.begin();
  EXPECT_TRUE((*it).status[git::FileStatus::Conflicted])
      << (*it).status.to_string();
  // Check for conflicted status
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
