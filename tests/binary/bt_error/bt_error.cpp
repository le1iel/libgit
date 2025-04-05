#include <gtest/gtest.h>
#include <libgit/error.h>


TEST(jeff, jeff)
{
    std::error_code ec = GitErrc::example;
    std::cout << ec << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
