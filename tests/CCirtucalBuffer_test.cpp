#include "CCirtucalBuffer.h"
#include <gtest/gtest.h>

TEST(CCirtucalBufferTest, DefaultConstructor) {
  CCirtucalBuffer<int> buffer;
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_EQ(buffer.capacity(), 0);
  EXPECT_TRUE(buffer.empty());
}

TEST(CCirtucalBufferTest, CapacityConstructor) {
  const int capacity = 5;
  CCirtucalBuffer<int> buffer(capacity);
  EXPECT_EQ(buffer.capacity(), capacity);
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_TRUE(buffer.empty());
}

TEST(CCirtucalBufferTest, SizeAndValueConstructor) {
  const int size = 5;
  const int value = 1;
  CCirtucalBuffer<int> buffer(size, value);
  EXPECT_EQ(buffer.size(), size);
  EXPECT_EQ(buffer.capacity(), size);
  EXPECT_FALSE(buffer.empty());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(buffer[i], value);
  }
}

TEST(CCirtucalBufferTest, CopyConstructor) {
  const int size = 5;
  const int value = 1;
  CCirtucalBuffer<int> buffer1(size, value);
  CCirtucalBuffer<int> buffer2(buffer1);
  EXPECT_EQ(buffer2.size(), buffer1.size());
  EXPECT_EQ(buffer2.capacity(), buffer1.capacity());
  EXPECT_FALSE(buffer2.empty());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(buffer2[i], buffer1[i]);
  }
}

TEST(CCirtucalBufferTest, MoveConstructor) {
  const int size = 5;
  const int value = 1;
  CCirtucalBuffer<int> buffer1(size, value);
  CCirtucalBuffer<int> buffer2(std::move(buffer1));
  EXPECT_EQ(buffer2.size(), size);
  EXPECT_EQ(buffer2.capacity(), size);
  EXPECT_FALSE(buffer2.empty());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(buffer2[i], value);
  }
  EXPECT_EQ(buffer1.size(), 0);
  EXPECT_EQ(buffer1.capacity(), 0);
  EXPECT_TRUE(buffer1.empty());
}

TEST(CCirtucalBufferTest, CopyAssignment) {
  const int size = 5;
  const int value = 1;
  CCirtucalBuffer<int> buffer1(size, value);
  CCirtucalBuffer<int> buffer2;
  buffer2 = buffer1;
  EXPECT_EQ(buffer2.size(), buffer1.size());
  EXPECT_EQ(buffer2.capacity(), buffer1.capacity());
  EXPECT_FALSE(buffer2.empty());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(buffer2[i], buffer1[i]);
  }
}

TEST(CCirtucalBufferTest, MoveAssignment) {
  const int size = 5;
  const int value = 1;
  CCirtucalBuffer<int> buffer1(size, value);
  CCirtucalBuffer<int> buffer2;
  buffer2 = std::move(buffer1);
  EXPECT_EQ(buffer2.size(), size);
  EXPECT_EQ(buffer2.capacity(), size);
  EXPECT_FALSE(buffer2.empty());
  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(buffer2[i], value);
  }
  EXPECT_EQ(buffer1.size(), 0);
  EXPECT_EQ(buffer1.capacity(), 0);
  EXPECT_TRUE(buffer1.empty());
}

TEST(CCirtucalBuffer, Insert) {
  CCirtucalBuffer<int> buffer(3);

  // Test inserting at the beginning
  buffer.insert(buffer.begin(), 1);
  ASSERT_EQ(buffer.size(), 1);
  ASSERT_EQ(buffer[0], 1);

  // Test inserting at the end
  buffer.insert(buffer.end(), 2);
  ASSERT_EQ(buffer.size(), 2);
  ASSERT_EQ(buffer[1], 2);

  // Test inserting in the middle
  buffer.insert(buffer.begin() + 1, 3);
  ASSERT_EQ(buffer.size(), 3);
  ASSERT_EQ(buffer[0], 1);
  ASSERT_EQ(buffer[1], 3);
  ASSERT_EQ(buffer[2], 2);

  // Test inserting multiple elements at the begin
  buffer.insert(buffer.begin(), 2, 4);
  ASSERT_EQ(buffer.size(), 3);
  ASSERT_EQ(buffer[0], 1);
  ASSERT_EQ(buffer[1], 3);
  ASSERT_EQ(buffer[2], 2);

  // Test inserting multiple elements at the middle
  buffer.insert(buffer.begin() + 2, 2, 4);
  ASSERT_EQ(buffer.size(), 3);
  ASSERT_EQ(buffer[0], 4);
  ASSERT_EQ(buffer[1], 4);
  ASSERT_EQ(buffer[2], 2);

  buffer.insert(buffer.begin() + 2, 5);
  ASSERT_EQ(buffer.size(), 3);
  ASSERT_EQ(buffer[0], 4);
  ASSERT_EQ(buffer[1], 5);
  ASSERT_EQ(buffer[2], 2);
}

TEST(CCirtucalBuffer, Emplace) {
  CCirtucalBuffer<std::pair<int, char>> buffer(3);

  // Test emplacing at the beginning
  buffer.emplace(buffer.begin(), 1, 'a');
  ASSERT_EQ(buffer.size(), 1);
  ASSERT_EQ(buffer[1].first, 1);
  ASSERT_EQ(buffer[1].second, 'a');

  // Test emplacing at the end
  buffer.emplace(buffer.end(), 2, 'b');
  ASSERT_EQ(buffer.size(), 2);
  ASSERT_EQ(buffer[1].first, 2);
  ASSERT_EQ(buffer[1].second, 'b');

  // Test emplacing in the middle
  buffer.emplace(buffer.begin() + 1, 3, 'c');
  ASSERT_EQ(buffer.size(), 3);
  ASSERT_EQ(buffer[0].first, 1);
  ASSERT_EQ(buffer[0].second, 'a');
  ASSERT_EQ(buffer[1].first, 3);
  ASSERT_EQ(buffer[1].second, 'c');
  ASSERT_EQ(buffer[2].first, 2);
  ASSERT_EQ(buffer[2].second, 'b');
}
