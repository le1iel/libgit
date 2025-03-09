#include <gtest/gtest.h>
#include <iostream>
#include <libgit/repository.h>
#include <git2.h>
#include <string.h>
#include <string_view>

class basic : public ::testing::Test {

    public:
  static void SetUpTestSuite() {
    git_libgit2_init();
  }

  void SetUp()
  {
    std::string command { "git init "};
    command.append("/tmp/something");

    system(command.c_str());
  }

  static void TearDownTestSuite() {
    git_libgit2_shutdown();
  }

};

// Demonstrate some basic assertions.
TEST_F(basic, basic) {
    std::cout << "hello" << std::endl;
  std::string repo_path{"/tmp/not_a_repo"};
  EXPECT_FALSE(git::Repository::Open(repo_path));
}

TEST_F(basic, huh) {
  std::string repo_path{"/tmp/something"};
  EXPECT_TRUE(git::Repository::Open(repo_path));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
