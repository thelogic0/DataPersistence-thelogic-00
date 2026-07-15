#pragma once

#include "Json.h"
#include <optional>
#include <string>

class ProductionQueueItem {
public:
    ProductionQueueItem() = default;
    ProductionQueueItem(std::string orderNo, std::string sampleId, int shortageQty, int actualProductionQty,
        double totalProductionTime, std::string queuedAt);

    const std::string& getOrderNo() const;
    const std::string& getSampleId() const;
    int getShortageQty() const;
    int getActualProductionQty() const;
    double getTotalProductionTime() const;
    const std::string& getQueuedAt() const;
    const std::optional<std::string>& getStartedAt() const;
    const std::optional<std::string>& getCompletedAt() const;

    void setStartedAt(const std::optional<std::string>& value);
    void setCompletedAt(const std::optional<std::string>& value);

    std::string toJson() const;
    std::string toDisplayString() const;

    static ProductionQueueItem fromJson(JsonParser& parser);

private:
    std::string order_no_;
    std::string sample_id_;
    int shortage_qty_ = 0;
    int actual_production_qty_ = 0;
    double total_production_time_ = 0.0;
    std::string queued_at_;
    std::optional<std::string> started_at_;
    std::optional<std::string> completed_at_;
};
