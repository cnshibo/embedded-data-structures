#include <gtest/gtest.h>
#include "../include/util/StaticMap.h"

// Test fixture class
class StaticMapTest : public ::testing::Test {
protected:
    static const size_t TABLE_SIZE = 100;
    typedef uint16_t KeyType;
    typedef int ValueType;
    
    // Custom hash function for uint16_t keys
    static size_t custom_hash_function(const KeyType& key) {
        return static_cast<size_t>(key) % TABLE_SIZE;
    }
    
    util::StaticMap<KeyType, ValueType, TABLE_SIZE> map;
    
    void SetUp() override {
        // Create a new map for each test
        map = util::StaticMap<KeyType, ValueType, TABLE_SIZE>(custom_hash_function);
    }
};

TEST_F(StaticMapTest, InitialState) {
    EXPECT_FALSE(map.contains(123));  // Should not contain any keys initially
}

TEST_F(StaticMapTest, InsertAndGet) {
    KeyType key = 123;
    ValueType value = 42;
    
    // Test insertion
    EXPECT_TRUE(map.insert(key, &value));
    
    // Test retrieval
    ValueType* retrieved = map.get(key);
    EXPECT_NE(retrieved, nullptr);
    EXPECT_EQ(*retrieved, value);
}

TEST_F(StaticMapTest, InsertDuplicate) {
    KeyType key = 123;
    ValueType value1 = 42;
    ValueType value2 = 84;
    
    // Insert first value
    EXPECT_TRUE(map.insert(key, &value1));
    
    // Insert second value with same key
    EXPECT_TRUE(map.insert(key, &value2));
    
    // Should get the second value
    ValueType* retrieved = map.get(key);
    EXPECT_NE(retrieved, nullptr);
    EXPECT_EQ(*retrieved, value2);
}

TEST_F(StaticMapTest, Remove) {
    KeyType key = 123;
    ValueType value = 42;
    
    // Insert and verify
    EXPECT_TRUE(map.insert(key, &value));
    EXPECT_TRUE(map.contains(key));
    
    // Remove and verify
    EXPECT_TRUE(map.remove(key));
    EXPECT_FALSE(map.contains(key));
}

TEST_F(StaticMapTest, RemoveNonExistent) {
    KeyType key = 123;
    EXPECT_FALSE(map.remove(key));  // Should fail to remove non-existent key
}

TEST_F(StaticMapTest, CollisionHandling) {
    // Create a hash function that will cause collisions
    auto collision_hash = [](const KeyType& key) { return 0; };
    util::StaticMap<KeyType, ValueType, TABLE_SIZE> collision_map(collision_hash);
    
    KeyType key1 = 1;
    KeyType key2 = 2;
    ValueType value1 = 42;
    ValueType value2 = 84;
    
    // Both keys should hash to the same value
    EXPECT_TRUE(collision_map.insert(key1, &value1));
    EXPECT_TRUE(collision_map.insert(key2, &value2));
    
    // Both values should be retrievable
    ValueType* retrieved1 = collision_map.get(key1);
    ValueType* retrieved2 = collision_map.get(key2);
    
    EXPECT_NE(retrieved1, nullptr);
    EXPECT_NE(retrieved2, nullptr);
    EXPECT_EQ(*retrieved1, value1);
    EXPECT_EQ(*retrieved2, value2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 