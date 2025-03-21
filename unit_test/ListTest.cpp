#include <gtest/gtest.h>
#include "../include/util/List.h"

// Test node class
class TestNode {
public:
    util::list_node_t _list;
    int data;
    
    TestNode(int d) : data(d) {}
};

class ListTest : public ::testing::Test {
protected:
    util::List<TestNode> list;
    
    void SetUp() override {
        // Clear list before each test
        while (!list.empty()) {
            TestNode* node = list.pop_front();
            delete node;
        }
    }
    
    void TearDown() override {
        // Clean up any remaining nodes
        while (!list.empty()) {
            TestNode* node = list.pop_front();
            delete node;
        }
    }
};

TEST_F(ListTest, InitialState) {
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_EQ(list.begin(), list.end());
}

TEST_F(ListTest, PushBackAndPopFront) {
    // Create and push a node
    TestNode* node = new TestNode(42);
    list.push_back(node);
    
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 1);
    
    // Pop and verify
    TestNode* popped = list.pop_front();
    EXPECT_EQ(popped->data, 42);
    EXPECT_TRUE(list.empty());
    
    delete popped;
}

TEST_F(ListTest, MultiplePushAndPop) {
    // Push multiple nodes
    for (int i = 0; i < 5; ++i) {
        TestNode* node = new TestNode(i);
        list.push_back(node);
        EXPECT_EQ(list.size(), i + 1);
    }
    
    // Pop all nodes and verify order
    for (int i = 0; i < 5; ++i) {
        TestNode* node = list.pop_front();
        EXPECT_EQ(node->data, i);
        delete node;
        EXPECT_EQ(list.size(), 4 - i);
    }
    
    EXPECT_TRUE(list.empty());
}

TEST_F(ListTest, Remove) {
    // Create and push nodes
    TestNode* node1 = new TestNode(1);
    TestNode* node2 = new TestNode(2);
    TestNode* node3 = new TestNode(3);
    
    list.push_back(node1);
    list.push_back(node2);
    list.push_back(node3);
    
    // Remove middle node
    list.remove(node2);
    EXPECT_EQ(list.size(), 2);
    
    // Verify remaining nodes
    TestNode* first = list.pop_front();
    TestNode* second = list.pop_front();
    
    EXPECT_EQ(first->data, 1);
    EXPECT_EQ(second->data, 3);
    
    delete first;
    delete second;
    delete node2;  // Clean up removed node
}

TEST_F(ListTest, PopBack) {
    // Create and push nodes
    TestNode* node1 = new TestNode(1);
    TestNode* node2 = new TestNode(2);
    TestNode* node3 = new TestNode(3);
    
    list.push_back(node1);
    list.push_back(node2);
    list.push_back(node3);
    
    // Pop from back
    TestNode* back = list.pop_back();
    EXPECT_EQ(back->data, 3);
    EXPECT_EQ(list.size(), 2);
    
    delete back;
    
    // Verify remaining nodes
    TestNode* first = list.pop_front();
    TestNode* second = list.pop_front();
    
    EXPECT_EQ(first->data, 1);
    EXPECT_EQ(second->data, 2);
    
    delete first;
    delete second;
}

TEST_F(ListTest, EmptyListOperations) {
    EXPECT_EQ(list.pop_front(), nullptr);
    EXPECT_EQ(list.pop_back(), nullptr);
}

TEST_F(ListTest, Iterator) {
    // Create and push nodes
    TestNode* node1 = new TestNode(1);
    TestNode* node2 = new TestNode(2);
    TestNode* node3 = new TestNode(3);
    
    list.push_back(node1);
    list.push_back(node2);
    list.push_back(node3);
    
    // Iterate and verify
    int expected[] = {1, 2, 3};
    int i = 0;
    
    for (util::list_node_t* pos = list.begin(); pos != list.end(); pos = pos->next) {
        TestNode* node = reinterpret_cast<TestNode*>(pos);
        EXPECT_EQ(node->data, expected[i++]);
    }
    
    // Clean up
    while (!list.empty()) {
        TestNode* node = list.pop_front();
        delete node;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 