#include "OrderRepository.h"
#include "Json.h"
#include <algorithm>
#include <fstream>
#include <sstream>

OrderRepository::OrderRepository(std::string filePath) : filePath_(std::move(filePath)) {
    load();
}

void OrderRepository::load() {
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
            orders_.push_back(Order::fromJson(parser));
        }
        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
}

void OrderRepository::save() const {
    std::ofstream out(filePath_);
    out << "[\n";
    for (size_t i = 0; i < orders_.size(); ++i) {
        out << orders_[i].toJson();
        out << (i + 1 < orders_.size() ? ",\n" : "\n");
    }
    out << "]\n";
}

const std::vector<Order>& OrderRepository::getAll() const {
    return orders_;
}

Order* OrderRepository::findById(const std::string& orderNo) {
    auto it = std::find_if(orders_.begin(), orders_.end(),
        [&orderNo](const Order& o) { return o.getOrderNo() == orderNo; });
    return it == orders_.end() ? nullptr : &(*it);
}

bool OrderRepository::add(const Order& order) {
    if (findById(order.getOrderNo()) != nullptr) {
        return false;
    }
    orders_.push_back(order);
    save();
    return true;
}

bool OrderRepository::remove(const std::string& orderNo) {
    auto it = std::find_if(orders_.begin(), orders_.end(),
        [&orderNo](const Order& o) { return o.getOrderNo() == orderNo; });
    if (it == orders_.end()) {
        return false;
    }
    orders_.erase(it);
    save();
    return true;
}

bool OrderRepository::update(const Order& order) {
    Order* existing = findById(order.getOrderNo());
    if (!existing) {
        return false;
    }
    *existing = order;
    save();
    return true;
}
