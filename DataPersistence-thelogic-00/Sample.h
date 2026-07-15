#pragma once

#include "Json.h"
#include <string>

class Sample {
public:
    Sample() = default;
    Sample(std::string sampleId, std::string name, double avgProductionTime, double yieldRate, int currentStock);

    const std::string& getSampleId() const;
    const std::string& getName() const;
    double getAvgProductionTime() const;
    double getYieldRate() const;
    int getCurrentStock() const;

    void setName(const std::string& value);
    void setAvgProductionTime(double value);
    void setYieldRate(double value);
    void setCurrentStock(int value);

    std::string toJson() const;
    std::string toDisplayString() const;

    static Sample fromJson(JsonParser& parser);

private:
    std::string sample_id_;
    std::string name_;
    double avg_production_time_ = 0.0;
    double yield_rate_ = 0.0;
    int current_stock_ = 0;
};
