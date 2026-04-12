#include <gtest/gtest.h>

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
