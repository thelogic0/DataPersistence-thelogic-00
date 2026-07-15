#pragma once

#include "Json.h"
#include <string>

class Order {
public:
    Order() = default;
    Order(std::string orderNo, std::string sampleId, std::string customerName, int quantity,
          std::string status, std::string createdAt, std::string updatedAt);

    const std::string& getOrderNo() const;
    const std::string& getSampleId() const;
    const std::string& getCustomerName() const;
    int getQuantity() const;
    const std::string& getStatus() const;
    const std::string& getCreatedAt() const;
    const std::string& getUpdatedAt() const;

    void setStatus(const std::string& value);
    void setUpdatedAt(const std::string& value);

    std::string toJson() const;
    std::string toDisplayString() const;

    static Order fromJson(JsonParser& parser);

private:
    std::string order_no_;
    std::string sample_id_;
    std::string customer_name_;
    int quantity_ = 0;
    std::string status_;
    std::string created_at_;
    std::string updated_at_;
};
