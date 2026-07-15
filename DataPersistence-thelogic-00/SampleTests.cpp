#include <gtest/gtest.h>
#include "Sample.h"
#include "Json.h"

TEST(SampleTest, GettersReturnConstructedValues) {
    Sample s("S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480);
    EXPECT_EQ(s.getSampleId(), "S-001");
    EXPECT_EQ(s.getName(), "실리콘 웨이퍼-8인치");
    EXPECT_DOUBLE_EQ(s.getAvgProductionTime(), 0.5);
    EXPECT_DOUBLE_EQ(s.getYieldRate(), 0.92);
    EXPECT_EQ(s.getCurrentStock(), 480);
}

TEST(SampleTest, SettersUpdateFields) {
    Sample s;
    s.setName("GaN 에피텍셜-4인치");
    s.setAvgProductionTime(0.3);
    s.setYieldRate(0.78);
    s.setCurrentStock(220);

    EXPECT_EQ(s.getName(), "GaN 에피텍셜-4인치");
    EXPECT_DOUBLE_EQ(s.getAvgProductionTime(), 0.3);
    EXPECT_DOUBLE_EQ(s.getYieldRate(), 0.78);
    EXPECT_EQ(s.getCurrentStock(), 220);
}

TEST(SampleTest, ToJsonAndFromJsonRoundTrip) {
    Sample s("S-003", "SiC 파워기판-6인치", 0.8, 0.92, 30);
    std::string json = s.toJson();

    JsonParser parser(json);
    parser.skipWhitespace();
    Sample parsed = Sample::fromJson(parser);

    EXPECT_EQ(parsed.getSampleId(), "S-003");
    EXPECT_EQ(parsed.getName(), "SiC 파워기판-6인치");
    EXPECT_DOUBLE_EQ(parsed.getAvgProductionTime(), 0.8);
    EXPECT_DOUBLE_EQ(parsed.getYieldRate(), 0.92);
    EXPECT_EQ(parsed.getCurrentStock(), 30);
}

TEST(SampleTest, ToDisplayStringContainsAllFields) {
    Sample s("S-001", "실리콘 웨이퍼-8인치", 0.5, 0.92, 480);
    std::string display = s.toDisplayString();

    EXPECT_NE(display.find("S-001"), std::string::npos);
    EXPECT_NE(display.find("실리콘 웨이퍼-8인치"), std::string::npos);
}
