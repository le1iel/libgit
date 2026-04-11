#include <gtest/gtest.h>

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

TEST_F(status_options_ut, new_file) {
  gitxx::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());
  gitxx::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status.any());
}

TEST_F(status_options_ut, modified_file) {
  gitxx::GitCommands internalRepo{};

  std::string modified_file = "test.txt";

  ASSERT_TRUE(internalRepo.createFile(modified_file, "content"));
  ASSERT_TRUE(internalRepo.add(modified_file));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));
  ASSERT_TRUE(internalRepo.modifyFile(modified_file, "new content"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());
  gitxx::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status[gitxx::FileStatus::WtModified]);
}

TEST_F(status_options_ut, deleted_file) {
  gitxx::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));
  ASSERT_TRUE(internalRepo.deleteFile("test.txt"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());
  gitxx::Repository repo = std::move(repoRes.value());

  auto statusRes = repo.status();
  auto status = std::move(statusRes).value();

  EXPECT_TRUE(status.begin().operator*().status[gitxx::FileStatus::WtDeleted]);
}

TEST_F(status_options_ut, clean_repo_no_entries) {
  gitxx::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));
  ASSERT_TRUE(internalRepo.commit("Initial commit"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto statusRes = repoRes->status();
  ASSERT_TRUE(statusRes.has_value());

  EXPECT_EQ(statusRes->begin(), statusRes->end());
}

TEST_F(status_options_ut, file_lookup) {
  gitxx::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags =
          gitxx::FlagField<gitxx::StatusFlags>{
              gitxx::StatusFlags::IncludeUntracked},
  });
  ASSERT_TRUE(statusRes.has_value());

  auto entry = statusRes->file("test.txt");
  EXPECT_TRUE(entry.status.test(gitxx::FileStatus::WtNew));
}

TEST_F(status_options_ut, staged_new_file_head_to_index) {
  gitxx::GitCommands internalRepo{};
  ASSERT_TRUE(internalRepo.makeEmptyCommit("one"));
  ASSERT_TRUE(internalRepo.createFile("test.txt", "content"));
  ASSERT_TRUE(internalRepo.add("test.txt"));

  auto repoRes = gitxx::Repository::Open(internalRepo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .show = gitxx::StatusShow::Index,
  });
  ASSERT_TRUE(statusRes.has_value());

  auto entry = *(statusRes->begin());
  ASSERT_TRUE(entry.head_to_index.has_value());
  EXPECT_EQ(entry.head_to_index->status, gitxx::DiffDeltaStatus::Added);
  EXPECT_EQ(entry.head_to_index->new_file.path, "test.txt");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
