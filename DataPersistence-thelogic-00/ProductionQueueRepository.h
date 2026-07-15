#pragma once

#include "ProductionQueueItem.h"
#include <string>
#include <vector>

class ProductionQueueRepository {
public:
    explicit ProductionQueueRepository(std::string filePath);

    const std::vector<ProductionQueueItem>& getAll() const;
    ProductionQueueItem* findById(const std::string& orderNo);

    bool add(const ProductionQueueItem& item);
    bool remove(const std::string& orderNo);
    bool update(const ProductionQueueItem& item);

    void save() const;

private:
    void load();

    std::string filePath_;
    std::vector<ProductionQueueItem> items_;
};
