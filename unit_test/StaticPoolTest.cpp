#include <gtest/gtest.h>
#include "../include/util/StaticPool.h"

class StaticPoolTest : public ::testing::Test {
protected:
    static const size_t POOL_SIZE = 10;
    util::StaticPool<int, POOL_SIZE> pool;
};

TEST_F(StaticPoolTest, InitialState) {
    EXPECT_EQ(pool.allocate(), nullptr);  // Should return nullptr when empty
}

TEST_F(StaticPoolTest, AllocateAndDeallocate) {
    // Allocate an integer
    int* num = pool.allocate();
    EXPECT_NE(num, nullptr);
    
    // Set value
    *num = 42;
    
    // Deallocate
    EXPECT_TRUE(pool.deallocate(num));
    
    // Try to deallocate again (should fail)
    EXPECT_FALSE(pool.deallocate(num));
}

TEST_F(StaticPoolTest, MultipleAllocations) {
    int* numbers[POOL_SIZE];
    
    // Allocate all available slots
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        numbers[i] = pool.allocate();
        EXPECT_NE(numbers[i], nullptr);
        *numbers[i] = static_cast<int>(i);
    }
    
    // Try to allocate one more (should fail)
    EXPECT_EQ(pool.allocate(), nullptr);
    
    // Deallocate all
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        EXPECT_TRUE(pool.deallocate(numbers[i]));
    }
}

TEST_F(StaticPoolTest, DeallocateInvalidPointer) {
    int invalid_value = 42;
    int* invalid_ptr = &invalid_value;
    
    // Try to deallocate pointer not from the pool
    EXPECT_FALSE(pool.deallocate(invalid_ptr));
}

TEST_F(StaticPoolTest, ReuseAfterDeallocation) {
    // Allocate and deallocate
    int* num = pool.allocate();
    EXPECT_NE(num, nullptr);
    *num = 42;
    EXPECT_TRUE(pool.deallocate(num));
    
    // Allocate again (should reuse the same memory)
    int* reused = pool.allocate();
    EXPECT_NE(reused, nullptr);
    EXPECT_EQ(reused, num);  // Should be the same pointer
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 