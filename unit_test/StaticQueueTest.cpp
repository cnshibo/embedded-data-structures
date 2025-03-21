#include <gtest/gtest.h>
#include "../include/util/StaticQueue.h"

class StaticQueueTest : public ::testing::Test {
protected:
    static const size_t QUEUE_SIZE = 5;
    util::StaticQueue<int, QUEUE_SIZE> queue;
    
    void SetUp() override {
        // Clear queue before each test
        while (!queue.empty()) {
            int* front = queue.front();
            queue.pop();
            delete front;
        }
    }
    
    void TearDown() override {
        // Clean up any remaining elements
        while (!queue.empty()) {
            int* front = queue.front();
            queue.pop();
            delete front;
        }
    }
};

TEST_F(StaticQueueTest, InitialState) {
    EXPECT_TRUE(queue.empty());
    EXPECT_FALSE(queue.full());
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.front(), nullptr);
    EXPECT_EQ(queue.back(), nullptr);
}

TEST_F(StaticQueueTest, PushAndPop) {
    // Push an element
    int* value = new int(42);
    EXPECT_TRUE(queue.push(value));
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1);
    
    // Check front and back
    EXPECT_EQ(*queue.front(), 42);
    EXPECT_EQ(*queue.back(), 42);
    
    // Pop the element
    queue.pop();
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
    
    delete value;
}

TEST_F(StaticQueueTest, MultiplePushAndPop) {
    // Push multiple elements
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        int* value = new int(i);
        EXPECT_TRUE(queue.push(value));
        EXPECT_EQ(queue.size(), i + 1);
        EXPECT_EQ(*queue.back(), i);
    }
    
    // Verify queue is full
    EXPECT_TRUE(queue.full());
    
    // Try to push one more (should fail)
    int* extra = new int(99);
    EXPECT_FALSE(queue.push(extra));
    delete extra;
    
    // Pop all elements
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        EXPECT_EQ(*queue.front(), i);
        int* front = queue.front();
        queue.pop();
        delete front;
        EXPECT_EQ(queue.size(), QUEUE_SIZE - i - 1);
    }
    
    EXPECT_TRUE(queue.empty());
}

TEST_F(StaticQueueTest, CircularBehavior) {
    // Fill queue
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        int* value = new int(i);
        queue.push(value);
    }
    
    // Pop some elements
    for (int i = 0; i < 2; ++i) {
        int* front = queue.front();
        queue.pop();
        delete front;
    }
    
    // Push new elements
    for (int i = 0; i < 2; ++i) {
        int* value = new int(i + QUEUE_SIZE);
        EXPECT_TRUE(queue.push(value));
    }
    
    // Verify order
    int expected[] = {2, 3, 4, 0, 1};
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        EXPECT_EQ(*queue.front(), expected[i]);
        int* front = queue.front();
        queue.pop();
        delete front;
    }
    
    EXPECT_TRUE(queue.empty());
}

TEST_F(StaticQueueTest, EmptyQueueOperations) {
    // Pop from empty queue
    queue.pop();  // Should not crash
    
    // Front and back should be nullptr
    EXPECT_EQ(queue.front(), nullptr);
    EXPECT_EQ(queue.back(), nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 