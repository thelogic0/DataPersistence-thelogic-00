#include "ProductionQueueRepository.h"
#include "Json.h"
#include <algorithm>
#include <fstream>
#include <sstream>

ProductionQueueRepository::ProductionQueueRepository(std::string filePath) : filePath_(std::move(filePath)) {
    load();
}

void ProductionQueueRepository::load() {
    std::ifstream in(filePath_);
    if (!in.is_open()) {
        return;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string text = buffer.str();

    JsonParser parser(text);
    parser.skipWhitespace();
    if (parser.isEnd() || parser.peek() != '[') {
        return;
    }
    parser.advance(); // '['

    while (true) {
        parser.skipWhitespace();
        if (parser.isEnd() || parser.peek() == ']') {
            break;
        }
        if (parser.peek() == '{') {
            items_.push_back(ProductionQueueItem::fromJson(parser));
        }
        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
}

void ProductionQueueRepository::save() const {
    std::ofstream out(filePath_);
    out << "[\n";
    for (size_t i = 0; i < items_.size(); ++i) {
        out << items_[i].toJson();
        out << (i + 1 < items_.size() ? ",\n" : "\n");
    }
    out << "]\n";
}

const std::vector<ProductionQueueItem>& ProductionQueueRepository::getAll() const {
    return items_;
}

ProductionQueueItem* ProductionQueueRepository::findById(const std::string& orderNo) {
    auto it = std::find_if(items_.begin(), items_.end(),
        [&orderNo](const ProductionQueueItem& p) { return p.getOrderNo() == orderNo; });
    return it == items_.end() ? nullptr : &(*it);
}

bool ProductionQueueRepository::add(const ProductionQueueItem& item) {
    if (findById(item.getOrderNo()) != nullptr) {
        return false;
    }
    items_.push_back(item);
    save();
    return true;
}

bool ProductionQueueRepository::remove(const std::string& orderNo) {
    auto it = std::find_if(items_.begin(), items_.end(),
        [&orderNo](const ProductionQueueItem& p) { return p.getOrderNo() == orderNo; });
    if (it == items_.end()) {
        return false;
    }
    items_.erase(it);
    save();
    return true;
}

bool ProductionQueueRepository::update(const ProductionQueueItem& item) {
    ProductionQueueItem* existing = findById(item.getOrderNo());
    if (!existing) {
        return false;
    }
    *existing = item;
    save();
    return true;
}
