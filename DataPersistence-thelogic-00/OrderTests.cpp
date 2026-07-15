#include <gtest/gtest.h>
#include "Order.h"
#include "Json.h"

TEST(OrderTest, GettersReturnConstructedValues) {
    Order o("ORD-20260416-0043", "S-003", "삼성전자 파운드리", 200, "RESERVED",
        "2026-04-16 09:32:15", "2026-04-16 09:32:15");
    EXPECT_EQ(o.getOrderNo(), "ORD-20260416-0043");
    EXPECT_EQ(o.getSampleId(), "S-003");
    EXPECT_EQ(o.getCustomerName(), "삼성전자 파운드리");
    EXPECT_EQ(o.getQuantity(), 200);
    EXPECT_EQ(o.getStatus(), "RESERVED");
    EXPECT_EQ(o.getCreatedAt(), "2026-04-16 09:32:15");
    EXPECT_EQ(o.getUpdatedAt(), "2026-04-16 09:32:15");
}

TEST(OrderTest, SettersUpdateStatusAndUpdatedAt) {
    Order o;
    o.setStatus("PRODUCING");
    o.setUpdatedAt("2026-04-16 10:00:00");

    EXPECT_EQ(o.getStatus(), "PRODUCING");
    EXPECT_EQ(o.getUpdatedAt(), "2026-04-16 10:00:00");
}

TEST(OrderTest, ToJsonAndFromJsonRoundTrip) {
    Order o("ORD-20260416-0043", "S-003", "삼성전자 파운드리", 200, "RESERVED",
        "2026-04-16 09:32:15", "2026-04-16 09:32:15");
    std::string json = o.toJson();

    JsonParser parser(json);
    parser.skipWhitespace();
    Order parsed = Order::fromJson(parser);

    EXPECT_EQ(parsed.getOrderNo(), "ORD-20260416-0043");
    EXPECT_EQ(parsed.getSampleId(), "S-003");
    EXPECT_EQ(parsed.getCustomerName(), "삼성전자 파운드리");
    EXPECT_EQ(parsed.getQuantity(), 200);
    EXPECT_EQ(parsed.getStatus(), "RESERVED");
}

TEST(OrderTest, ToDisplayStringContainsAllFields) {
    Order o("ORD-0001", "S-001", "LG이노텍", 300, "RESERVED", "t1", "t1");
    std::string display = o.toDisplayString();

    EXPECT_NE(display.find("ORD-0001"), std::string::npos);
    EXPECT_NE(display.find("LG이노텍"), std::string::npos);
    EXPECT_NE(display.find("RESERVED"), std::string::npos);
}
