#include <gtest/gtest.h>

#include <gitxx/error.hpp>

class bt_error : public ::testing::TestWithParam<GitErrc> {};

TEST_P(bt_error, message) {
  std::error_code error_code = GetParam();
  std::string message = error_code.message();
  EXPECT_FALSE(message.empty());
}

INSTANTIATE_TEST_SUITE_P(
    bt_error, bt_error,
    ::testing::Values(
        GitErrc::not_found, GitErrc::exists, GitErrc::ambiguous, GitErrc::bufs,
        GitErrc::user, GitErrc::bare_repo, GitErrc::unborn_branch,
        GitErrc::unmerged, GitErrc::non_fast_forward, GitErrc::invalid_spec,
        GitErrc::conflict, GitErrc::locked, GitErrc::modified, GitErrc::auth,
        GitErrc::certificate, GitErrc::applied, GitErrc::peel, GitErrc::eof,
        GitErrc::invalid, GitErrc::uncommitted, GitErrc::directory,
        GitErrc::merge_conflict, GitErrc::passthrough, GitErrc::iterover,
        GitErrc::retry, GitErrc::mismatch, GitErrc::index_dirty,
        GitErrc::apply_fail, GitErrc::owner, GitErrc::timeout,
        GitErrc::unchanged, GitErrc::not_supported, GitErrc::readonly,
        static_cast<GitErrc>(1)));

TEST(bt_error_category, name_non_empty) {
  EXPECT_STRNE(gitErrcCategory.name(), "");
}

TEST(bt_error_category, make_error_code_value) {
  std::error_code error = make_error_code(GitErrc::not_found);
  EXPECT_EQ(error.value(), static_cast<int>(GitErrc::not_found));
  EXPECT_STREQ(error.category().name(), gitErrcCategory.name());
}

TEST(bt_error_formatter, formats_to_message) {
  std::string formatted = std::format("{}", GitErrc::not_found);
  std::string expected = make_error_code(GitErrc::not_found).message();
  EXPECT_EQ(formatted, expected);
}

TEST(bt_error_formatter, formatted_non_empty) {
  std::string formatted = std::format("{}", GitErrc::not_found);
  EXPECT_FALSE(formatted.empty());
}
