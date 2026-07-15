#include <gtest/gtest.h>
#include "OrderRepository.h"
#include <cstdio>

namespace {

    const char* TEST_FILE = "test_orders.json";

    class OrderRepositoryTest : public ::testing::Test {
    protected:
        void SetUp() override {
            std::remove(TEST_FILE);
        }
        void TearDown() override {
            std::remove(TEST_FILE);
        }
    };

}

TEST_F(OrderRepositoryTest, StartsEmptyWhenFileMissing) {
    OrderRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(OrderRepositoryTest, AddPersistsOrder) {
    OrderRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.add(Order("ORD-0001", "S-001", "LG이노텍", 300, "RESERVED", "t1", "t1")));

    ASSERT_EQ(repo.getAll().size(), 1u);
    EXPECT_EQ(repo.getAll()[0].getCustomerName(), "LG이노텍");
}

TEST_F(OrderRepositoryTest, AddRejectsDuplicateId) {
    OrderRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.add(Order("ORD-0001", "S-001", "A", 1, "RESERVED", "t1", "t1")));
    EXPECT_FALSE(repo.add(Order("ORD-0001", "S-002", "B", 2, "RESERVED", "t2", "t2")));
    EXPECT_EQ(repo.getAll().size(), 1u);
}

TEST_F(OrderRepositoryTest, ReloadsPersistedData) {
    {
        OrderRepository repo(TEST_FILE);
        repo.add(Order("ORD-0001", "S-001", "LG이노텍", 300, "RESERVED", "t1", "t1"));
    }

    OrderRepository reloaded(TEST_FILE);
    ASSERT_EQ(reloaded.getAll().size(), 1u);
    EXPECT_EQ(reloaded.getAll()[0].getCustomerName(), "LG이노텍");
}

TEST_F(OrderRepositoryTest, FindByIdReturnsPointerOrNull) {
    OrderRepository repo(TEST_FILE);
    repo.add(Order("ORD-0001", "S-001", "A", 1, "RESERVED", "t1", "t1"));

    EXPECT_NE(repo.findById("ORD-0001"), nullptr);
    EXPECT_EQ(repo.findById("ORD-9999"), nullptr);
}

TEST_F(OrderRepositoryTest, UpdateMutatesAndPersists) {
    OrderRepository repo(TEST_FILE);
    repo.add(Order("ORD-0001", "S-001", "A", 1, "RESERVED", "t1", "t1"));

    Order* existing = repo.findById("ORD-0001");
    Order updated(*existing);
    updated.setStatus("PRODUCING");
    updated.setUpdatedAt("t2");
    EXPECT_TRUE(repo.update(updated));
    EXPECT_FALSE(repo.update(Order("ORD-9999", "S-001", "A", 1, "RESERVED", "t1", "t1")));

    Order* o = repo.findById("ORD-0001");
    ASSERT_NE(o, nullptr);
    EXPECT_EQ(o->getStatus(), "PRODUCING");
    EXPECT_EQ(o->getUpdatedAt(), "t2");
}

TEST_F(OrderRepositoryTest, RemoveDeletesOrder) {
    OrderRepository repo(TEST_FILE);
    repo.add(Order("ORD-0001", "S-001", "A", 1, "RESERVED", "t1", "t1"));

    EXPECT_TRUE(repo.remove("ORD-0001"));
    EXPECT_EQ(repo.getAll().size(), 0u);
    EXPECT_FALSE(repo.remove("ORD-0001"));
}
