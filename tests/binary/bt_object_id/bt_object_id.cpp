#include <gtest/gtest.h>

#include <gitxx/object_id.hpp>
#include <sstream>

TEST(stream, string) {
  gitxx::ObjectId::Tag id_data{std::byte{'a'}};

  gitxx::ObjectId id{id_data};

  std::stringstream ss{};

  ss << id;

  EXPECT_EQ(ss.view(), id.id());
}

TEST(compare, eq) {
  gitxx::ObjectId::Tag id_data{std::byte{'a'}};

  gitxx::ObjectId id1{id_data};
  gitxx::ObjectId id2{id_data};

  EXPECT_EQ(id1, id2);
}

TEST(compare, neq) {
  gitxx::ObjectId::Tag id_data1{std::byte{'a'}};
  gitxx::ObjectId::Tag id_data2{std::byte{'b'}};

  gitxx::ObjectId id1{id_data1};
  gitxx::ObjectId id2{id_data2};

  EXPECT_NE(id1, id2);
}
