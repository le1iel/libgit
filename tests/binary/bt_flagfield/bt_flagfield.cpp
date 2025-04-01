#include <gtest/gtest.h>
#include <flagfield.h>

namespace git {

enum class TestFlags : std::uint32_t {
    Flag1 = 1 << 0,
    Flag2 = 1 << 1,
    Flag3 = 1 << 2
};

} // namespace git

class flagfield_ut : public ::testing::Test {
public:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
};

TEST(flagfield_ut, single_flag) {
    git::FlagField<git::TestFlags, 3> flags(1);
    EXPECT_TRUE(flags[git::TestFlags::Flag1]);
    EXPECT_FALSE(flags[git::TestFlags::Flag2]);
    EXPECT_FALSE(flags[git::TestFlags::Flag3]);

    EXPECT_TRUE(flags.at(0));
    EXPECT_FALSE(flags.at(1));
    EXPECT_FALSE(flags.at(2));
}

TEST(flagfield_ut, multiple_flags) {
    git::FlagField<git::TestFlags, 3> flags(
        static_cast<std::uint32_t>(git::TestFlags::Flag1) |
        static_cast<std::uint32_t>(git::TestFlags::Flag2)
    );
    EXPECT_TRUE(flags[git::TestFlags::Flag1]);
    EXPECT_TRUE(flags[git::TestFlags::Flag2]);
    EXPECT_FALSE(flags[git::TestFlags::Flag3]);
}

TEST(flagfield_ut, all_flags) {
    git::FlagField<git::TestFlags, 3> flags(
        static_cast<std::uint32_t>(git::TestFlags::Flag1) |
        static_cast<std::uint32_t>(git::TestFlags::Flag2) |
        static_cast<std::uint32_t>(git::TestFlags::Flag3)
    );
    EXPECT_TRUE(flags[git::TestFlags::Flag1]);
    EXPECT_TRUE(flags[git::TestFlags::Flag2]);
    EXPECT_TRUE(flags[git::TestFlags::Flag3]);
}

TEST(flagfield_ut, no_flags) {
    git::FlagField<git::TestFlags, 3> flags(0);
    EXPECT_FALSE(flags[git::TestFlags::Flag1]);
    EXPECT_FALSE(flags[git::TestFlags::Flag2]);
    EXPECT_FALSE(flags[git::TestFlags::Flag3]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 