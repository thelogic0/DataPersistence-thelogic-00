#include <gtest/gtest.h>
#include "ProductionQueueItem.h"
#include "Json.h"

TEST(ProductionQueueItemTest, GettersReturnConstructedValues) {
    ProductionQueueItem p("ORD-0043", "S-003", 170, 185, 165.0, "2026-04-16 09:32:15");
    EXPECT_EQ(p.getOrderNo(), "ORD-0043");
    EXPECT_EQ(p.getSampleId(), "S-003");
    EXPECT_EQ(p.getShortageQty(), 170);
    EXPECT_EQ(p.getActualProductionQty(), 185);
    EXPECT_DOUBLE_EQ(p.getTotalProductionTime(), 165.0);
    EXPECT_EQ(p.getQueuedAt(), "2026-04-16 09:32:15");
    EXPECT_FALSE(p.getStartedAt().has_value());
    EXPECT_FALSE(p.getCompletedAt().has_value());
}

TEST(ProductionQueueItemTest, SettersAcceptOptionalValues) {
    ProductionQueueItem p;
    p.setStartedAt(std::optional<std::string>("2026-04-16 09:00:00"));
    p.setCompletedAt(std::nullopt);

    ASSERT_TRUE(p.getStartedAt().has_value());
    EXPECT_EQ(*p.getStartedAt(), "2026-04-16 09:00:00");
    EXPECT_FALSE(p.getCompletedAt().has_value());
}

TEST(ProductionQueueItemTest, ToJsonAndFromJsonRoundTripWithNullFields) {
    ProductionQueueItem p("ORD-0043", "S-003", 170, 185, 165.0, "2026-04-16 09:32:15");
    std::string json = p.toJson();

    JsonParser parser(json);
    parser.skipWhitespace();
    ProductionQueueItem parsed = ProductionQueueItem::fromJson(parser);

    EXPECT_EQ(parsed.getOrderNo(), "ORD-0043");
    EXPECT_EQ(parsed.getShortageQty(), 170);
    EXPECT_EQ(parsed.getActualProductionQty(), 185);
    EXPECT_DOUBLE_EQ(parsed.getTotalProductionTime(), 165.0);
    EXPECT_FALSE(parsed.getStartedAt().has_value());
    EXPECT_FALSE(parsed.getCompletedAt().has_value());
}

TEST(ProductionQueueItemTest, ToJsonAndFromJsonRoundTripWithSetValues) {
    ProductionQueueItem p("ORD-0043", "S-003", 170, 185, 165.0, "2026-04-16 09:32:15");
    p.setStartedAt(std::optional<std::string>("2026-04-16 09:32:15"));
    p.setCompletedAt(std::optional<std::string>("2026-04-16 12:17:15"));
    std::string json = p.toJson();

    JsonParser parser(json);
    parser.skipWhitespace();
    ProductionQueueItem parsed = ProductionQueueItem::fromJson(parser);

    ASSERT_TRUE(parsed.getStartedAt().has_value());
    ASSERT_TRUE(parsed.getCompletedAt().has_value());
    EXPECT_EQ(*parsed.getStartedAt(), "2026-04-16 09:32:15");
    EXPECT_EQ(*parsed.getCompletedAt(), "2026-04-16 12:17:15");
}
