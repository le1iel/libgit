#include <gtest/gtest.h>

#include <flagfield.hpp>
#include <git_commands.hpp>
#include <gitxx/init.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>

class status_options_ut : public ::testing::Test {
 public:
  static void SetUpTestSuite() { init_libgit(); }

  static void TearDownTestSuite() { deinit_libgit(); }
};

// Updated new_file_untracked test case
TEST_F(status_options_ut, new_file_untracked) {
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.makeEmptyCommit("one"));
  ASSERT_TRUE(internal_repo.createFile("test.txt", "content"));

  auto repo_res =
      gitxx::Repository::Open(std::string_view(internal_repo.path().c_str()));
  ASSERT_TRUE(repo_res.has_value());

  const gitxx::Repository& repo = repo_res.value();
  auto statusRes = repo.status(gitxx::StatusOptions{
      .flags =
          gitxx::FlagField<gitxx::StatusFlags>{
              gitxx::StatusFlags::IncludeUntracked},
  });
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  EXPECT_EQ(status.end() - status.begin(), 1);

  auto status_it = *(status.begin());
  EXPECT_TRUE(status_it.status.test(gitxx::FileStatus::WtNew));
}

// Updated new_file_untracked test case
TEST_F(status_options_ut, new_file_untracked_index) {
  gitxx::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.makeEmptyCommit("start"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.printStatus());
  ASSERT_TRUE(internalRepo.add("test.txt"));

  auto repo_res =
      gitxx::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repo_res.has_value());

  const gitxx::Repository& repo = repo_res.value();
  auto status_res = repo.status(gitxx::StatusOptions{
      .show = gitxx::StatusShow::Index,
      .flags =
          gitxx::FlagField<gitxx::StatusFlags>{
              gitxx::StatusFlags::IncludeUntracked,
              gitxx::StatusFlags::UpdateIndex,
              gitxx::StatusFlags::RenamesHeadToIndex},
  });
  ASSERT_TRUE(status_res.has_value());
  const auto& status = status_res.value();

  EXPECT_EQ(status.end() - status.begin(), 1);

  auto status_it = *(status.begin());
  EXPECT_TRUE(status_it.status.test(gitxx::FileStatus::Current))
      << status_it.status.to_string();
}

TEST_F(status_options_ut, 2_new_file_untracked) {
  // setup
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.makeEmptyCommit("one"));
  ASSERT_TRUE(internal_repo.createFile("test.txt", "content"));
  ASSERT_TRUE(internal_repo.createFile("test2.txt", "content"));

  auto repo_res =
      gitxx::Repository::Open(std::string_view(internal_repo.path().c_str()));
  ASSERT_TRUE(repo_res.has_value());

  const gitxx::Repository& repo = repo_res.value();

  auto status_res = repo.status(gitxx::StatusOptions{
      .flags =
          gitxx::FlagField<gitxx::StatusFlags>{
              gitxx::StatusFlags::IncludeUntracked},
  });
  ASSERT_TRUE(status_res.has_value());

  const auto& status = status_res.value();
  auto other_status{status};

  EXPECT_EQ(status.end() - status.begin(), 2);
}

// Updated modified_file test case
TEST_F(status_options_ut, modified_file) {
  gitxx::GitCommands internal_repo{};

  std::string modified_file = "test.txt";

  ASSERT_TRUE(internal_repo.createFile(modified_file, "content"));
  ASSERT_TRUE(internal_repo.add(modified_file));
  ASSERT_TRUE(internal_repo.commit("Initial commit"));
  ASSERT_TRUE(internal_repo.modifyFile(modified_file, "new content"));

  auto repo_res =
      gitxx::Repository::Open(std::string_view(internal_repo.path().c_str()));
  ASSERT_TRUE(repo_res.has_value());

  gitxx::Repository repo = std::move(repo_res.value());

  auto status_res = repo.status();
  auto status = std::move(status_res).value();

  EXPECT_TRUE(
      status.begin()
          .operator*()
          .status[gitxx::FileStatus::WtModified]);  // Check for modified status
}

// Updated deleted_file test case
TEST_F(status_options_ut, deleted_file) {
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.createFile("test.txt", "content"));
  ASSERT_TRUE(internal_repo.add("test.txt"));
  ASSERT_TRUE(internal_repo.commit("Initial commit"));

  ASSERT_TRUE(internal_repo.deleteFile("test.txt"));

  auto repo_res =
      gitxx::Repository::Open(std::string_view(internal_repo.path().c_str()));
  ASSERT_TRUE(repo_res.has_value());

  gitxx::Repository repo = std::move(repo_res.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(
      status.begin()
          .operator*()
          .status[gitxx::FileStatus::WtDeleted]);  // Check for deleted status
}

TEST_F(status_options_ut, renamed_file) {
  gitxx::GitCommands internalRepo{};

  ASSERT_TRUE(internalRepo.createFile("old_name.txt", "content"));
  ASSERT_TRUE(internalRepo.add("old_name.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));

  ASSERT_TRUE(internalRepo.renameFile("old_name.txt", "new_name.txt"));

  auto repoRes =
      gitxx::Repository::Open(std::string_view(internalRepo.path().c_str()));
  ASSERT_TRUE(repoRes.has_value());

  gitxx::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status(gitxx::StatusOptions{
      .show = gitxx::StatusShow::IndexAndWorkdir,
      .flags =
          gitxx::FlagField<gitxx::StatusFlags>{
              gitxx::StatusFlags::RenamesFromRewrites,
              gitxx::StatusFlags::RenamesIndexToWorkdir,
              gitxx::StatusFlags::RenamesHeadToIndex},
  });

  auto status = std::move(statusRes).value();

  EXPECT_EQ(status.end() - status.begin(), 1);

  auto status_it = status.begin();
  EXPECT_TRUE((*status_it).status[gitxx::FileStatus::IndexRenamed])
      << (*status_it).status.to_string();
}

// TEST_F(status_ut, type_changed_file) {
//   gitxx::GitCommands internalRepo{};

//   ASSERT_TRUE(internalRepo.createFile("test", "content"));
//   ASSERT_TRUE(internalRepo.add("test"));
//   ASSERT_TRUE(internalRepo.commit("Initial commit"));
//   ASSERT_TRUE(internalRepo.deleteFile("test"));
//   ASSERT_TRUE(internalRepo.createDir("test"));
//   ASSERT_TRUE(internalRepo.createFile("test/a", "content"));

//   auto repoRes = gitxx::Repository::Open(internalRepo.path().string());
//   ASSERT_TRUE(repoRes.has_value());

//   gitxx::Repository repo = std::move(repoRes.value());

//   auto statusRes = repo.status();
//   auto status = std::move(statusRes).value();

//   auto it = status.begin();
//   EXPECT_TRUE((*it).status[gitxx::FileStatus::IndexTypeChanged])
//       << (*it).status.to_string();
//   internalRepo.printStatus();
//   // Check for type-changed status
// }

TEST_F(status_options_ut, ignored_file) {
  gitxx::GitCommands internalRepo{};

  ASSERT_TRUE(internalRepo.createFile(".gitignore", "*.log"));
  ASSERT_TRUE(internalRepo.add(".gitignore"));
  ASSERT_TRUE(internalRepo.commit("Added gitignore"));

  ASSERT_TRUE(internalRepo.createFile("ignored.log", "log content"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  gitxx::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status(gitxx::StatusOptions{
      .show = gitxx::StatusShow::IndexAndWorkdir,
      .flags =
          gitxx::FlagField<gitxx::StatusFlags>{
              gitxx::StatusFlags::IncludeIgnored},
  });
  auto status = std::move(statusRes).value();

  auto status_it = status.begin();
  EXPECT_TRUE((*status_it).status[gitxx::FileStatus::Ignored])
      << (*status_it).status.to_string();
  // Check for ignored status
}

TEST_F(status_options_ut, conflicting_file) {
  gitxx::GitCommands internalRepo{};

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

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  gitxx::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  auto status_it = status.begin();
  EXPECT_TRUE((*status_it).status[gitxx::FileStatus::Conflicted])
      << (*status_it).status.to_string();
  // Check for conflicted status
}
