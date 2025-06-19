#include <gtest/gtest.h>

#include <gitxx/object_id.hpp>
#include <sstream>

TEST(stream, string) {
  std::array<std::uint8_t, 20> id_data{'a', 20};

  git::ObjectId id{id_data};

  std::stringstream ss{};

  ss << id;

  EXPECT_EQ(ss.view(), id.id());
}

TEST(compare, eq) {
  std::array<std::uint8_t, 20> id_data{'a', 20};

  git::ObjectId id1{id_data};
  git::ObjectId id2{id_data};

  EXPECT_EQ(id1, id2);
}

TEST(compare, neq) {
  std::array<std::uint8_t, 20> id_data1{'a', 20};
  std::array<std::uint8_t, 20> id_data2{'b', 20};

  git::ObjectId id1{id_data1};
  git::ObjectId id2{id_data2};

  EXPECT_NE(id1, id2);
}
