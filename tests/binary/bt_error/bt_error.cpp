#include <gtest/gtest.h>

#include <gitxx/error.hpp>

TEST(bt_error, message) {
  std::error_code ec = GitErrc::eof;
  std::cout << ec << std::endl;
  std::cout << ec.message() << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
