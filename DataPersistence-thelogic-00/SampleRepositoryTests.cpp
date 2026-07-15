#include <gtest/gtest.h>
#include "SampleRepository.h"
#include <cstdio>

namespace {

    const char* TEST_FILE = "test_samples.json";

    class SampleRepositoryTest : public ::testing::Test {
    protected:
        void SetUp() override {
            std::remove(TEST_FILE);
        }
        void TearDown() override {
            std::remove(TEST_FILE);
        }
    };

}

TEST_F(SampleRepositoryTest, StartsEmptyWhenFileMissing) {
    SampleRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.getAll().empty());
}

TEST_F(SampleRepositoryTest, AddPersistsSample) {
    SampleRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.add(Sample("S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480)));

    ASSERT_EQ(repo.getAll().size(), 1u);
    EXPECT_EQ(repo.getAll()[0].getName(), "실리콘 웨이퍼-8인치");
}

TEST_F(SampleRepositoryTest, AddRejectsDuplicateId) {
    SampleRepository repo(TEST_FILE);
    EXPECT_TRUE(repo.add(Sample("S-001", "A", 0.5, 0.9, 10)));
    EXPECT_FALSE(repo.add(Sample("S-001", "B", 0.1, 0.1, 1)));
    EXPECT_EQ(repo.getAll().size(), 1u);
}

TEST_F(SampleRepositoryTest, ReloadsPersistedData) {
    {
        SampleRepository repo(TEST_FILE);
        repo.add(Sample("S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480));
    }

    SampleRepository reloaded(TEST_FILE);
    ASSERT_EQ(reloaded.getAll().size(), 1u);
    EXPECT_EQ(reloaded.getAll()[0].getName(), "실리콘 웨이퍼-8인치");
}

TEST_F(SampleRepositoryTest, FindByIdReturnsPointerOrNull) {
    SampleRepository repo(TEST_FILE);
    repo.add(Sample("S-001", "A", 0.5, 0.9, 10));

    EXPECT_NE(repo.findById("S-001"), nullptr);
    EXPECT_EQ(repo.findById("S-999"), nullptr);
}

TEST_F(SampleRepositoryTest, UpdateMutatesAndPersists) {
    SampleRepository repo(TEST_FILE);
    repo.add(Sample("S-001", "A", 0.5, 0.9, 10));

    Sample updated("S-001", "A2", 0.6, 0.95, 20);
    EXPECT_TRUE(repo.update(updated));
    EXPECT_FALSE(repo.update(Sample("S-999", "X", 0.1, 0.1, 1)));

    Sample* s = repo.findById("S-001");
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(s->getName(), "A2");
    EXPECT_EQ(s->getCurrentStock(), 20);
}

TEST_F(SampleRepositoryTest, RemoveDeletesSample) {
    SampleRepository repo(TEST_FILE);
    repo.add(Sample("S-001", "A", 0.5, 0.9, 10));

    EXPECT_TRUE(repo.remove("S-001"));
    EXPECT_EQ(repo.getAll().size(), 0u);
    EXPECT_FALSE(repo.remove("S-001"));
}
