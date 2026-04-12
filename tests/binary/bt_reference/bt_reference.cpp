#include <gtest/gtest.h>

#include <string>
#include <gitxx/reference.hpp>
#include <gitxx/repository.hpp>

#include "git_commands.hpp"

class bt_reference : public ::testing::Test {};

TEST_F(bt_reference, name_returns_full_ref_name) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_EQ(head->name(), "refs/heads/main");
}

TEST_F(bt_reference, shorthand_returns_branch_name) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_EQ(head->shorthand(), "main");
}

TEST_F(bt_reference, is_branch_true_for_head) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_TRUE(head->isBranch());
}

TEST_F(bt_reference, is_tag_false_for_branch) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_FALSE(head->isTag());
}

TEST_F(bt_reference, is_note_false_for_branch) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_FALSE(head->isNote());
}

TEST_F(bt_reference, is_remote_false_for_local_branch) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_FALSE(head->isRemote());
}

TEST_F(bt_reference, type_is_direct_for_resolved_head) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_EQ(head->type(), gitxx::ReferenceType::Direct);
}

TEST_F(bt_reference, resolve_succeeds_on_direct_ref) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_TRUE(head->resolve().has_value());
}

TEST_F(bt_reference, name_unchanged_after_resolve) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  const std::string name_before = head->name();
  ASSERT_TRUE(head->resolve().has_value());

  EXPECT_EQ(head->name(), name_before);
}

TEST_F(bt_reference, resolve_fails_on_unborn_symbolic_ref) {
  const std::string test_dir =
      std::string(TEST_OUTPUT_DIR) + "/gitxx_unborn_head_resolve_test";

  git_libgit2_init();
  git_repository* raw_repo = nullptr;
  ASSERT_EQ(git_repository_init(&raw_repo, test_dir.c_str(), /*is_bare=*/0), 0);

  git_reference* raw_ref = nullptr;
  ASSERT_EQ(git_reference_lookup(&raw_ref, raw_repo, "HEAD"), 0);

  bool result_has_value{};
  {
    gitxx::Reference ref{raw_ref};
    result_has_value = ref.resolve().has_value();
    // ref destructs here, freeing raw_ref before the repo is freed
  }

  git_repository_free(raw_repo);
  git_libgit2_shutdown();

  EXPECT_FALSE(result_has_value);
}

TEST_F(bt_reference, name_changes_after_branch_switch) {
  gitxx::GitCommands repo{};
  ASSERT_TRUE(repo.makeEmptyCommit("1"));
  ASSERT_TRUE(repo.createBranchAndCheckout("feature"));

  auto repoRes = gitxx::Repository::Open(repo.path());
  ASSERT_TRUE(repoRes.has_value());

  auto head = repoRes->head();
  ASSERT_TRUE(head.has_value());

  EXPECT_EQ(head->name(), "refs/heads/feature");
  EXPECT_EQ(head->shorthand(), "feature");
}
