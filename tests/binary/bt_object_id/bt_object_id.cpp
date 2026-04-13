#include <gtest/gtest.h>

#include <gitxx/object_id.hpp>
#include <sstream>

TEST(stream, string) {
  gitxx::ObjectId::Tag id_data{'a'};
  const gitxx::ObjectId object_id{id_data};

  std::stringstream stream{};
  stream << object_id;
  EXPECT_EQ(stream.view(), object_id.id());
}

TEST(compare, eq) {
  gitxx::ObjectId::Tag id_data{'a'};

  const gitxx::ObjectId id1{id_data};
  const gitxx::ObjectId id2{id_data};

  EXPECT_EQ(id1, id2);
}

TEST(compare, neq) {
  gitxx::ObjectId::Tag id_data1{'a'};
  gitxx::ObjectId::Tag id_data2{'b'};

  const gitxx::ObjectId id1{id_data1};
  const gitxx::ObjectId id2{id_data2};

  EXPECT_NE(id1, id2);
}

TEST(id, full_length) {
  gitxx::ObjectId::Tag id_data{};
  const gitxx::ObjectId object_id{id_data};
  EXPECT_EQ(object_id.id().size(), TAG_LENGTH);
}
