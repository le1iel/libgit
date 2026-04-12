#include <gtest/gtest.h>

#include <array>
#include <format>
#include <string_view>
#include <utility>
#include <flagfield.hpp>

using gitxx::FlagField;

/// @brief Test enum
enum class TestFlags: uint8_t { Flag1 = 0, Flag2 = 1, Flag3 = 2 };

/// @brief C style flag enum
enum class OldStyle: uint8_t {
  Flag1 = 1U << 0U,
  Flag2 = 1U << 1U,
  Flag3 = 1U << 2U,
};

TEST(constructor, from_old_style) {
  auto data = static_cast<OldStyle>(static_cast<unsigned int>(OldStyle::Flag1) |
                                    static_cast<unsigned int>(OldStyle::Flag2));

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

template <>
struct gitxx::FlagFieldEnumValues<TestFlags> {
  static constexpr std::array<std::pair<TestFlags, std::string_view>, 3> values = {{
    {TestFlags::Flag1, "Flag1"},
    {TestFlags::Flag2, "Flag2"},
    {TestFlags::Flag3, "Flag3"},
  }};
};

TEST(format, no_flags_set) {
  FlagField<TestFlags> flags(0);
  EXPECT_EQ(std::format("{}", flags), "[]");
}

TEST(format, single_flag) {
  FlagField<TestFlags> flags{TestFlags::Flag2};
  EXPECT_EQ(std::format("{}", flags), "[Flag2]");
}

TEST(format, multiple_flags) {
  FlagField<TestFlags> flags{TestFlags::Flag1, TestFlags::Flag3};
  EXPECT_EQ(std::format("{}", flags), "[Flag1, Flag3]");
}

TEST(format, fallback_no_specialization) {
  FlagField<OldStyle> flags{OldStyle::Flag1};
  // No FlagFieldEnumValues<OldStyle> specialization — falls back to bitset string
  EXPECT_EQ(std::format("{}", flags), "[" + flags.to_string() + "]");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
