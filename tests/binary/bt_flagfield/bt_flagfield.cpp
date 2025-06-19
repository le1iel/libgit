#include <gtest/gtest.h>

#include <flagfield.hpp>

using git::FlagField;

/// @brief Test enum
enum class TestFlags { Flag1 = 0, Flag2 = 1, Flag3 = 2 };

/// @brief C style flag enum
enum class OldStyle {
  Flag1 = 1 << 0,
  Flag2 = 1 << 1,
  Flag3 = 1 << 2,
};

TEST(constructor, from_old_style) {
  auto data = static_cast<OldStyle>(static_cast<int>(OldStyle::Flag1) |
                                    static_cast<int>(OldStyle::Flag2));

  FlagField<TestFlags> converted{static_cast<std::uint32_t>(data)};
  EXPECT_TRUE(converted.test(TestFlags::Flag1));
  EXPECT_TRUE(converted.test(TestFlags::Flag2));
  EXPECT_FALSE(converted.test(TestFlags::Flag3));
}

TEST(constructor, from_new_style) {
  FlagField<TestFlags> converted{TestFlags::Flag1};
  EXPECT_TRUE(converted.test(TestFlags::Flag1));
  EXPECT_FALSE(converted.test(TestFlags::Flag2));
  EXPECT_FALSE(converted.test(TestFlags::Flag3));
}

TEST(constructor, init_list) {
  EXPECT_EQ((FlagField<TestFlags>{TestFlags::Flag1}).value(), 1);
  EXPECT_EQ((FlagField<TestFlags>{TestFlags::Flag2}).value(), 2);
  EXPECT_EQ((FlagField<TestFlags>{TestFlags::Flag3}).value(), 4);
  EXPECT_EQ((FlagField<TestFlags>{TestFlags::Flag1, TestFlags::Flag2}).value(),
            3);
  EXPECT_EQ((FlagField<TestFlags>{TestFlags::Flag1, TestFlags::Flag2,
                                  TestFlags::Flag3})
                .value(),
            7);
}

TEST(indexing, read_single) {
  FlagField<TestFlags> flags(1);
  EXPECT_TRUE(flags[TestFlags::Flag1]);
  EXPECT_FALSE(flags[TestFlags::Flag2]);
  EXPECT_FALSE(flags[TestFlags::Flag3]);
}

TEST(indexing, read_multiple) {
  FlagField<TestFlags> flags(3);
  EXPECT_TRUE(flags[TestFlags::Flag1]);
  EXPECT_TRUE(flags[TestFlags::Flag2]);
  EXPECT_FALSE(flags[TestFlags::Flag3]);
}

TEST(indexing, write_single) {
  FlagField<TestFlags> flags{};
  flags[TestFlags::Flag1] = true;

  EXPECT_TRUE(flags.test(TestFlags::Flag1));
  EXPECT_FALSE(flags.test(TestFlags::Flag2));
  EXPECT_FALSE(flags.test(TestFlags::Flag3));
}

TEST(indexing, write_multiple) {
  FlagField<TestFlags> flags{};
  flags[TestFlags::Flag1] = true;
  flags[TestFlags::Flag3] = true;

  EXPECT_TRUE(flags.test(TestFlags::Flag1));
  EXPECT_FALSE(flags.test(TestFlags::Flag2));
  EXPECT_TRUE(flags.test(TestFlags::Flag3));
}

TEST(empty, no_flags) {
  FlagField<TestFlags> flags(0);
  EXPECT_TRUE(flags.none());
}

TEST(empty, with_flags) {
  FlagField<TestFlags> flags(1);
  EXPECT_FALSE(flags.none());
}

TEST(set, single) {
  FlagField<TestFlags> flags(0);
  EXPECT_TRUE(flags.none());

  flags.set(TestFlags::Flag1);

  EXPECT_TRUE(flags[TestFlags::Flag1]);
  EXPECT_FALSE(flags.test(TestFlags::Flag2));
  EXPECT_FALSE(flags.test(TestFlags::Flag3));
}

TEST(set, multiple) {
  FlagField<TestFlags> flags(0);
  EXPECT_TRUE(flags.none());

  flags.set(TestFlags::Flag1);
  flags.set(TestFlags::Flag3);

  EXPECT_TRUE(flags[TestFlags::Flag1]);
  EXPECT_FALSE(flags.test(TestFlags::Flag2));
  EXPECT_TRUE(flags.test(TestFlags::Flag3));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
