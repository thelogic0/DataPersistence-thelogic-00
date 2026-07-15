#pragma once

#include "Order.h"
#include <string>
#include <vector>

class OrderRepository {
public:
    explicit OrderRepository(std::string filePath);

    const std::vector<Order>& getAll() const;
    Order* findById(const std::string& orderNo);

    bool add(const Order& order);
    bool remove(const std::string& orderNo);
    bool update(const Order& order);

    void save() const;

private:
    void load();

    std::string filePath_;
    std::vector<Order> orders_;
};
