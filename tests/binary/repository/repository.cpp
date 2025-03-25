#include <gtest/gtest.h>
#include <libgit/repository.h>
#include <git2.h>

class repository_ut : public ::testing::Test {

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

TEST_F(repository_ut, open_non_repo) {
  std::string repo_path{"/tmp/not_a_repo"};
  EXPECT_FALSE(git::Repository::Open(repo_path));
}

TEST_F(repository_ut, open_success) {
  std::string repo_path{"/tmp/something"};
  EXPECT_TRUE(git::Repository::Open(repo_path));
}

TEST_F(repository_ut, open)
{
  std::string repo_path{"/tmp/something"};
  auto openRes = git::Repository::Open(repo_path);
  ASSERT_TRUE(openRes.has_value());

  EXPECT_FALSE(openRes->head().has_value());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
