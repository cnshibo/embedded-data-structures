#include <iostream>
#include <gtest/gtest.h>
#include "../include/util/CircleBuffer.h"

class CircleBufferTest : public ::testing::Test {
protected:
    static const size_t BufferSize = 10;
    util::CircleBuffer<BufferSize> buffer;
};

TEST_F(CircleBufferTest, InitialState) {
    EXPECT_TRUE(buffer.is_empty());
    EXPECT_FALSE(buffer.is_full());
    EXPECT_EQ(buffer.get_size(), 0);
    EXPECT_EQ(buffer.get_capacity(), BufferSize);
}

TEST_F(CircleBufferTest, PutAndGet) {
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    size_t data_length = sizeof(data);

    // Test putting data
    EXPECT_TRUE(buffer.put(data, data_length));
    EXPECT_EQ(buffer.get_size(), data_length);
    EXPECT_FALSE(buffer.is_empty());

    // Test getting data
    uint8_t received_data[BufferSize];
    EXPECT_TRUE(buffer.get(received_data, data_length));
    EXPECT_EQ(buffer.get_size(), 0);
    EXPECT_TRUE(buffer.is_empty());

    // Verify data integrity
    for (size_t i = 0; i < data_length; ++i) {
        EXPECT_EQ(received_data[i], data[i]);
    }
}

TEST_F(CircleBufferTest, BufferWrapAround) {
    uint8_t data1[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    uint8_t data2[] = { 0x06, 0x07, 0x08, 0x09, 0x0A };
    size_t data_length = sizeof(data1);

    // Fill buffer partially
    EXPECT_TRUE(buffer.put(data1, data_length));
    EXPECT_TRUE(buffer.put(data2, data_length));
    EXPECT_TRUE(buffer.is_full());

    // Get data and verify wrap-around
    uint8_t received_data[BufferSize];
    EXPECT_TRUE(buffer.get(received_data, BufferSize));
    
    // Verify first half
    for (size_t i = 0; i < data_length; ++i) {
        EXPECT_EQ(received_data[i], data1[i]);
    }
    
    // Verify second half
    for (size_t i = 0; i < data_length; ++i) {
        EXPECT_EQ(received_data[i + data_length], data2[i]);
    }
}

TEST_F(CircleBufferTest, OverflowProtection) {
    uint8_t data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    size_t data_length = sizeof(data);

    // Fill buffer to capacity
    for (size_t i = 0; i < BufferSize / data_length; ++i) {
        EXPECT_TRUE(buffer.put(data, data_length));
    }

    // Try to put more data
    EXPECT_FALSE(buffer.put(data, data_length));
    EXPECT_TRUE(buffer.is_full());
}

TEST_F(CircleBufferTest, UnderflowProtection) {
    uint8_t data[BufferSize];
    
    // Try to get data from empty buffer
    EXPECT_FALSE(buffer.get(data, 1));
    EXPECT_TRUE(buffer.is_empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 