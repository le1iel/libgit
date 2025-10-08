#include <gtest/gtest.h>

#include <gitxx/error.hpp>


class bt_error :public ::testing::TestWithParam<GitErrc> {};

TEST_P(bt_error, message) {
  std::error_code error_code = GetParam();
  std::string message = error_code.message();
  EXPECT_FALSE(message.empty());
}

INSTANTIATE_TEST_SUITE_P(
        bt_error,
        bt_error,
        ::testing::Values(
            GitErrc::not_found,
            GitErrc::exists,
            GitErrc::ambiguous,
            GitErrc::bufs,
            GitErrc::user,
            GitErrc::bare_repo,
            GitErrc::unborn_branch,
            GitErrc::unmerged,
            GitErrc::non_fast_forward,
            GitErrc::invalid_spec,
            GitErrc::conflict,
            GitErrc::locked,
            GitErrc::modified,
            GitErrc::auth,
            GitErrc::certificate,
            GitErrc::applied,
            GitErrc::peel,
            GitErrc::eof,
            GitErrc::invalid,
            GitErrc::uncommitted,
            GitErrc::directory,
            GitErrc::merge_conflict,
            GitErrc::passthrough,
            GitErrc::iterover,
            GitErrc::retry,
            GitErrc::mismatch,
            GitErrc::index_dirty,
            GitErrc::apply_fail,
            GitErrc::owner,
            GitErrc::timeout,
            GitErrc::unchanged,
            GitErrc::not_supported,
            GitErrc::readonly,
            static_cast<GitErrc>(1)
));
