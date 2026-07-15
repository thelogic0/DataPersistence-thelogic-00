#include "Order.h"

Order::Order(std::string orderNo, std::string sampleId, std::string customerName, int quantity,
    std::string status, std::string createdAt, std::string updatedAt)
    : order_no_(std::move(orderNo)), sample_id_(std::move(sampleId)), customer_name_(std::move(customerName)),
      quantity_(quantity), status_(std::move(status)), created_at_(std::move(createdAt)), updated_at_(std::move(updatedAt)) {}

const std::string& Order::getOrderNo() const { return order_no_; }
const std::string& Order::getSampleId() const { return sample_id_; }
const std::string& Order::getCustomerName() const { return customer_name_; }
int Order::getQuantity() const { return quantity_; }
const std::string& Order::getStatus() const { return status_; }
const std::string& Order::getCreatedAt() const { return created_at_; }
const std::string& Order::getUpdatedAt() const { return updated_at_; }

void Order::setStatus(const std::string& value) { status_ = value; }
void Order::setUpdatedAt(const std::string& value) { updated_at_ = value; }

std::string Order::toJson() const {
    std::string out;
    out += "  {\n";
    out += "    \"order_no\": \"" + JsonParser::escape(order_no_) + "\",\n";
    out += "    \"sample_id\": \"" + JsonParser::escape(sample_id_) + "\",\n";
    out += "    \"customer_name\": \"" + JsonParser::escape(customer_name_) + "\",\n";
    out += "    \"quantity\": " + std::to_string(quantity_) + ",\n";
    out += "    \"status\": \"" + JsonParser::escape(status_) + "\",\n";
    out += "    \"created_at\": \"" + JsonParser::escape(created_at_) + "\",\n";
    out += "    \"updated_at\": \"" + JsonParser::escape(updated_at_) + "\"\n";
    out += "  }";
    return out;
}

std::string Order::toDisplayString() const {
    return "주문번호: " + order_no_ +
        " | 시료: " + sample_id_ +
        " | 고객: " + customer_name_ +
        " | 수량: " + std::to_string(quantity_) +
        " | 상태: " + status_ +
        " | 생성: " + created_at_ +
        " | 수정: " + updated_at_;
}

Order Order::fromJson(JsonParser& parser) {
    Order o;
    parser.advance(); // '{'
    while (true) {
        parser.skipWhitespace();
        if (parser.isEnd() || parser.peek() == '}') {
            break;
        }
        std::string key = parser.parseString();
        parser.skipWhitespace();
        parser.advance(); // ':'
        parser.skipWhitespace();

        if (key == "order_no") {
            o.order_no_ = parser.parseString();
        }
        else if (key == "sample_id") {
            o.sample_id_ = parser.parseString();
        }
        else if (key == "customer_name") {
            o.customer_name_ = parser.parseString();
        }
        else if (key == "quantity") {
            o.quantity_ = parser.parseNumber();
        }
        else if (key == "status") {
            o.status_ = parser.parseString();
        }
        else if (key == "created_at") {
            o.created_at_ = parser.parseString();
        }
        else if (key == "updated_at") {
            o.updated_at_ = parser.parseString();
        }

        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
    parser.advance(); // '}'
    return o;
}
