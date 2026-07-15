#include <gtest/gtest.h>
#include "ProductionQueueRepository.h"
#include <cstdio>

namespace {

    const char* TEST_FILE = "test_production_queue.json";

    class ProductionQueueRepositoryTest : public ::testing::Test {
    protected:
        void SetUp() override {
            std::remove(TEST_FILE);
        }
        void TearDown() override {
            std::remove(TEST_FILE);
        }
    };

}

TEST_F(ProductionQueueRepositoryTest, StartsEmptyWhenFileMissing) {
    ProductionQueueRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(ProductionQueueRepositoryTest, AddPersistsItem) {
    ProductionQueueRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.add(ProductionQueueItem("ORD-0043", "S-003", 170, 185, 165.0, "t1")));

    ASSERT_EQ(repo.getAll().size(), 1u);
    EXPECT_EQ(repo.getAll()[0].getSampleId(), "S-003");
}

TEST_F(ProductionQueueRepositoryTest, AddRejectsDuplicateId) {
    ProductionQueueRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.add(ProductionQueueItem("ORD-0043", "S-003", 170, 185, 165.0, "t1")));
    EXPECT_FALSE(repo.add(ProductionQueueItem("ORD-0043", "S-004", 1, 1, 1.0, "t2")));
    EXPECT_EQ(repo.getAll().size(), 1u);
}

TEST_F(ProductionQueueRepositoryTest, ReloadsPersistedDataIncludingNullableFields) {
    {
        ProductionQueueRepository repo(TEST_FILE);
        ProductionQueueItem item("ORD-0043", "S-003", 170, 185, 165.0, "t1");
        item.setStartedAt(std::optional<std::string>("t1"));
        repo.add(item);
    }

    ProductionQueueRepository reloaded(TEST_FILE);
    ASSERT_EQ(reloaded.getAll().size(), 1u);
    ASSERT_TRUE(reloaded.getAll()[0].getStartedAt().has_value());
    EXPECT_EQ(*reloaded.getAll()[0].getStartedAt(), "t1");
    EXPECT_FALSE(reloaded.getAll()[0].getCompletedAt().has_value());
}

TEST_F(ProductionQueueRepositoryTest, FindByIdReturnsPointerOrNull) {
    ProductionQueueRepository repo(TEST_FILE);
    repo.add(ProductionQueueItem("ORD-0043", "S-003", 170, 185, 165.0, "t1"));

    EXPECT_NE(repo.findById("ORD-0043"), nullptr);
    EXPECT_EQ(repo.findById("ORD-9999"), nullptr);
}

TEST_F(ProductionQueueRepositoryTest, UpdateMutatesAndPersists) {
    ProductionQueueRepository repo(TEST_FILE);
    repo.add(ProductionQueueItem("ORD-0043", "S-003", 170, 185, 165.0, "t1"));

    ProductionQueueItem* existing = repo.findById("ORD-0043");
    ProductionQueueItem updated(*existing);
    updated.setStartedAt(std::optional<std::string>("t1"));
    updated.setCompletedAt(std::optional<std::string>("t2"));
    EXPECT_TRUE(repo.update(updated));
    EXPECT_FALSE(repo.update(ProductionQueueItem("ORD-9999", "S-003", 1, 1, 1.0, "t1")));

    ProductionQueueItem* p = repo.findById("ORD-0043");
    ASSERT_NE(p, nullptr);
    ASSERT_TRUE(p->getCompletedAt().has_value());
    EXPECT_EQ(*p->getCompletedAt(), "t2");
}

TEST_F(ProductionQueueRepositoryTest, RemoveDeletesItem) {
    ProductionQueueRepository repo(TEST_FILE);
    repo.add(ProductionQueueItem("ORD-0043", "S-003", 170, 185, 165.0, "t1"));

    EXPECT_TRUE(repo.remove("ORD-0043"));
    EXPECT_EQ(repo.getAll().size(), 0u);
    EXPECT_FALSE(repo.remove("ORD-0043"));
}
