#include "CCirtucalBufferExt.h"
#include <gtest/gtest.h>

TEST(CCirtucalBufferExtTest, EmptyBufferTest) {
  CCirtucalBufferExt<int> buffer;
  EXPECT_TRUE(buffer.empty());
  EXPECT_EQ(buffer.size(), (size_t) 0);
  EXPECT_EQ(buffer.capacity(), (size_t) 0);
  EXPECT_EQ(buffer.begin(), buffer.end());
}

TEST(CCirtucalBufferExtTest, SingleElementBufferTest) {
  CCirtucalBufferExt<int> buffer(1, 0);
  EXPECT_FALSE(buffer.empty());
  EXPECT_EQ(buffer.size(), 1);
  EXPECT_EQ(buffer.capacity(), 1);
  buffer[0] = 42;
  EXPECT_EQ(buffer[0], 42);
  EXPECT_EQ(*buffer.begin(), 42);
  EXPECT_EQ(++buffer.begin(), buffer.end());
}

TEST(CCirtucalBufferExtTest, MultipleElementsBufferTest) {
  CCirtucalBufferExt<int> buffer(5, 1);
  EXPECT_FALSE(buffer.empty());
  EXPECT_EQ(buffer.size(), 5);
  EXPECT_EQ(buffer.capacity(), 5);
  EXPECT_EQ(buffer[0], 1);
  EXPECT_EQ(buffer[4], 1);
  buffer[2] = 42;
  EXPECT_EQ(buffer[2], 42);
  EXPECT_EQ(*buffer.begin(), 1);
  EXPECT_EQ(*(buffer.end() - 1), 1);
}

TEST(CCirtucalBufferExtTest, MoveConstructorTest) {
  CCirtucalBufferExt<int> buffer(5, 1);
  CCirtucalBufferExt<int> buffer_moved(std::move(buffer));
  EXPECT_EQ(buffer_moved.size(), 5);
  EXPECT_EQ(buffer_moved.capacity(), 5);
  EXPECT_EQ(buffer_moved[0], 1);
  EXPECT_EQ(buffer_moved[4], 1);
  EXPECT_TRUE(buffer.empty());
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_EQ(buffer.capacity(), 0);
}


TEST(CriticalBufferExtTest, InsertTest) {
  CCirtucalBufferExt<int> buffer(3);
  buffer.insert(buffer.begin(), 1);
  buffer.insert(buffer.begin() + 1, 2);
  buffer.insert(buffer.begin() + 2, 3);

  // Insert element when buffer is full
  buffer.insert(buffer.end(), 4);
  EXPECT_EQ(buffer.size(), 4);
  EXPECT_EQ(buffer[0], 1);
  EXPECT_EQ(buffer[1], 2);
  EXPECT_EQ(buffer[2], 3);
  EXPECT_EQ(buffer[3], 4);

  // Insert element when buffer is not full
  buffer.insert(buffer.end(), 5);
  EXPECT_EQ(buffer.size(), 5);
  EXPECT_EQ(buffer[0], 1);
  EXPECT_EQ(buffer[1], 2);
  EXPECT_EQ(buffer[2], 3);
  EXPECT_EQ(buffer[3], 4);
  EXPECT_EQ(buffer[4], 5);

  // Insert element at the front of the buffer
  buffer.insert(buffer.begin(), 0);
  EXPECT_EQ(buffer.size(), 6);
  EXPECT_EQ(buffer[0], 0);
  EXPECT_EQ(buffer[1], 1);
  EXPECT_EQ(buffer[2], 2);
  EXPECT_EQ(buffer[3], 3);
  EXPECT_EQ(buffer[4], 4);
  EXPECT_EQ(buffer[5], 5);

}

TEST(CriticalBufferExtTest, EmplaceTest) {
  CCirtucalBufferExt<std::string> buffer(2);
  buffer.emplace(buffer.begin(), "hello");
  buffer.emplace(buffer.begin() + 1, "world");

  // Emplace element when buffer is full
  buffer.emplace(buffer.end(), "!");
  EXPECT_EQ(buffer.size(), 3);
  EXPECT_EQ(buffer[0], "hello");
  EXPECT_EQ(buffer[1], "world");
  EXPECT_EQ(buffer[2], "!");

  // Emplace element when buffer is not full
  buffer.emplace(buffer.end(), "goodbye");
  EXPECT_EQ(buffer.size(), 4);
  EXPECT_EQ(buffer[0], "hello");
  EXPECT_EQ(buffer[1], "world");
  EXPECT_EQ(buffer[2], "!");
  EXPECT_EQ(buffer[3], "goodbye");

  // Emplace element at the front of the buffer
  buffer.emplace(buffer.begin(), "hi");
  EXPECT_EQ(buffer.size(), 5);
  EXPECT_EQ(buffer[0], "hi");
  EXPECT_EQ(buffer[1], "hello");
  EXPECT_EQ(buffer[2], "world");
  EXPECT_EQ(buffer[3], "!");
  EXPECT_EQ(buffer[4], "goodbye");

}

TEST(CCirtucalBufferExtTest, SortedBufferTest) {
  CCirtucalBufferExt<int> buffer(3);
  buffer.insert(buffer.begin(), 3);
  buffer.insert(buffer.begin() + 1, 2);
  buffer.insert(buffer.begin() + 2, 1);
  std::sort(buffer.begin(), buffer.end());

  EXPECT_EQ(buffer[0], 1);
  EXPECT_EQ(buffer[1], 2);
  EXPECT_EQ(buffer[2], 3);
}