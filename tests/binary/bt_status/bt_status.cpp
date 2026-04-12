#include <gtest/gtest.h>

#include <filesystem>
#include <flagfield.hpp>
#include <git2.h>
#include <git_commands.hpp>
#include <gitxx/repository.hpp>
#include <gitxx/status.hpp>
#include <gitxx/status_options.hpp>

class status_options_ut : public ::testing::Test {};

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

TEST_F(status_options_ut, file_returns_empty_on_null_status_list) {
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.makeEmptyCommit("one"));
  ASSERT_TRUE(internal_repo.createFile("test.txt", "content"));

  auto repoRes = gitxx::Repository::Open(internal_repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());

  gitxx::Status movedStatus = std::move(statusRes.value());

  const auto result = statusRes.value().file("test.txt");
  EXPECT_FALSE(result.head_to_index.has_value());
  EXPECT_FALSE(result.index_to_workdir.has_value());
}

TEST_F(status_options_ut, begin_on_null_status_list_gives_empty_range) {
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.makeEmptyCommit("one"));
  ASSERT_TRUE(internal_repo.createFile("test.txt", "content"));

  auto repoRes = gitxx::Repository::Open(internal_repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());

  gitxx::Status movedStatus = std::move(statusRes.value());

  const auto& emptyStatus = statusRes.value();
  EXPECT_EQ(emptyStatus.begin(), emptyStatus.end());
}

TEST_F(status_options_ut, file_returns_empty_when_not_found) {
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.makeEmptyCommit("initial"));
  ASSERT_TRUE(internal_repo.createFile("present.txt", "content"));

  auto repoRes = gitxx::Repository::Open(internal_repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());

  const auto& status = statusRes.value();
  const auto result = status.file("absent.txt");
  EXPECT_FALSE(result.head_to_index.has_value());
  EXPECT_FALSE(result.index_to_workdir.has_value());
}

TEST_F(status_options_ut, file_finds_staged_file_via_head_to_index) {
  gitxx::GitCommands internal_repo{};
  ASSERT_TRUE(internal_repo.makeEmptyCommit("initial"));
  ASSERT_TRUE(internal_repo.createFile("staged.txt", "content"));
  ASSERT_TRUE(internal_repo.add("staged.txt"));

  auto repoRes = gitxx::Repository::Open(internal_repo.path());
  ASSERT_TRUE(repoRes.has_value());

  const gitxx::Repository& repo = repoRes.value();
  auto statusRes = repo.status(gitxx::StatusOptions{
      .show = gitxx::StatusShow::Index,
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked,
          gitxx::StatusFlags::UpdateIndex,
          gitxx::StatusFlags::RenamesHeadToIndex},
  });
  ASSERT_TRUE(statusRes.has_value());

  const auto& status = statusRes.value();
  const auto entry = status.file("staged.txt");
  EXPECT_TRUE(entry.status.test(gitxx::FileStatus::IndexNew));
}

TEST_F(status_options_ut, status_fails_on_bare_repo) {
  const std::string test_dir =
      std::string(TEST_OUTPUT_DIR) + "/gitxx_bare_repo_status_test";
  std::filesystem::remove_all(test_dir);

  git_libgit2_init();
  git_repository* raw_repo = nullptr;
  ASSERT_EQ(git_repository_init(&raw_repo, test_dir.c_str(), /*is_bare=*/1), 0);
  git_repository_free(raw_repo);
  git_libgit2_shutdown();

  auto repoRes = gitxx::Repository::Open(test_dir);
  ASSERT_TRUE(repoRes.has_value());

  const gitxx::Repository& repo = repoRes.value();
  auto statusRes = repo.status();

  EXPECT_FALSE(statusRes.has_value());
}

class status_iterator_ut : public ::testing::Test {};

TEST_F(status_iterator_ut, pre_increment_advances) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin();
  ++it;
  EXPECT_EQ(status.end() - it, 2U);
}

TEST_F(status_iterator_ut, post_increment_returns_previous_position) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin();
  auto prev = it++;
  EXPECT_EQ(status.end() - prev, 3U);
  EXPECT_EQ(status.end() - it, 2U);
}

TEST_F(status_iterator_ut, pre_decrement_moves_back) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin() + 2;
  --it;
  EXPECT_EQ(status.end() - it, 2U);
}

TEST_F(status_iterator_ut, pre_decrement_at_begin_stays) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin();
  --it;
  EXPECT_EQ(status.end() - it, 3U);
}

TEST_F(status_iterator_ut, post_decrement_returns_previous_position) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin() + 2;
  auto prev = it--;
  EXPECT_EQ(status.end() - prev, 1U);
  EXPECT_EQ(status.end() - it, 2U);
}

TEST_F(status_iterator_ut, operator_plus_advances_by_n) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin() + 2;
  EXPECT_EQ(status.end() - it, 1U);
}

TEST_F(status_iterator_ut, operator_plus_clamped_to_end) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin() + 100;
  EXPECT_EQ(status.end() - it, 0U);
}

TEST_F(status_iterator_ut, operator_plus_equals_advances) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin();
  it += 2;
  EXPECT_EQ(status.end() - it, 1U);
}

TEST_F(status_iterator_ut, operator_minus_retreats_by_n) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = (status.begin() + 2) - 1;
  EXPECT_EQ(status.end() - it, 2U);
}

TEST_F(status_iterator_ut, operator_minus_clamped_to_begin) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin() - 100;
  EXPECT_EQ(status.end() - it, 3U);
}

TEST_F(status_iterator_ut, operator_minus_equals_retreats) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto it = status.begin() + 2;
  it -= 1;
  EXPECT_EQ(status.end() - it, 2U);
}

TEST_F(status_iterator_ut, operator_minus_equals_clamped_to_begin) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  constexpr std::size_t large_step = 100;
  auto iter = status.begin() + 1;
  iter -= large_step;
  EXPECT_EQ(status.end() - iter, 3U);
}

TEST_F(status_iterator_ut, spaceship_begin_less_than_end) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  EXPECT_LT(status.begin(), status.end());
}

TEST_F(status_iterator_ut, update_entry_skips_null_entry_from_null_status_list) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  auto iter = status.begin();
  auto movedIter = std::move(iter);
  // iter.m_statusList is now null; iter.m_statusCount is still 2.
  // ++iter: m_index becomes 1 < 2, git_status_byindex(nullptr, 1) == null.
  ++iter;
}

TEST_F(status_iterator_ut, spaceship_end_greater_than_begin) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  EXPECT_GT(status.end(), status.begin());
}

TEST_F(status_iterator_ut, spaceship_equal_index_compares_status_list) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  EXPECT_EQ(status.begin() <=> status.begin(), std::strong_ordering::equal);
}

TEST_F(status_iterator_ut, friend_sum_gives_index_sum) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createFile("a.txt", "a"));
  ASSERT_TRUE(repo.createFile("b.txt", "b"));
  ASSERT_TRUE(repo.createFile("c.txt", "c"));
  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());
  auto statusRes = repoRes->status(gitxx::StatusOptions{
      .flags = gitxx::FlagField<gitxx::StatusFlags>{
          gitxx::StatusFlags::IncludeUntracked}});
  ASSERT_TRUE(statusRes.has_value());
  const auto& status = statusRes.value();

  const auto it1 = status.begin() + 1;
  const auto it2 = status.begin() + 2;
  EXPECT_EQ(it1 + it2, 3U);
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
