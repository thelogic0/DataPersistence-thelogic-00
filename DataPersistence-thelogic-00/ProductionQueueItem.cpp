#include "ProductionQueueItem.h"

ProductionQueueItem::ProductionQueueItem(std::string orderNo, std::string sampleId, int shortageQty,
    int actualProductionQty, double totalProductionTime, std::string queuedAt)
    : order_no_(std::move(orderNo)), sample_id_(std::move(sampleId)), shortage_qty_(shortageQty),
      actual_production_qty_(actualProductionQty), total_production_time_(totalProductionTime),
      queued_at_(std::move(queuedAt)) {}

const std::string& ProductionQueueItem::getOrderNo() const { return order_no_; }
const std::string& ProductionQueueItem::getSampleId() const { return sample_id_; }
int ProductionQueueItem::getShortageQty() const { return shortage_qty_; }
int ProductionQueueItem::getActualProductionQty() const { return actual_production_qty_; }
double ProductionQueueItem::getTotalProductionTime() const { return total_production_time_; }
const std::string& ProductionQueueItem::getQueuedAt() const { return queued_at_; }
const std::optional<std::string>& ProductionQueueItem::getStartedAt() const { return started_at_; }
const std::optional<std::string>& ProductionQueueItem::getCompletedAt() const { return completed_at_; }

void ProductionQueueItem::setStartedAt(const std::optional<std::string>& value) { started_at_ = value; }
void ProductionQueueItem::setCompletedAt(const std::optional<std::string>& value) { completed_at_ = value; }

std::string ProductionQueueItem::toJson() const {
    std::string out;
    out += "  {\n";
    out += "    \"order_no\": \"" + JsonParser::escape(order_no_) + "\",\n";
    out += "    \"sample_id\": \"" + JsonParser::escape(sample_id_) + "\",\n";
    out += "    \"shortage_qty\": " + std::to_string(shortage_qty_) + ",\n";
    out += "    \"actual_production_qty\": " + std::to_string(actual_production_qty_) + ",\n";
    out += "    \"total_production_time\": " + std::to_string(total_production_time_) + ",\n";
    out += "    \"queued_at\": \"" + JsonParser::escape(queued_at_) + "\",\n";
    out += "    \"started_at\": " +
        (started_at_.has_value() ? ("\"" + JsonParser::escape(*started_at_) + "\"") : std::string("null")) + ",\n";
    out += "    \"completed_at\": " +
        (completed_at_.has_value() ? ("\"" + JsonParser::escape(*completed_at_) + "\"") : std::string("null")) + "\n";
    out += "  }";
    return out;
}

std::string ProductionQueueItem::toDisplayString() const {
    return "주문번호: " + order_no_ +
        " | 시료: " + sample_id_ +
        " | 부족분: " + std::to_string(shortage_qty_) +
        " | 실생산량: " + std::to_string(actual_production_qty_) +
        " | 소요시간(min): " + std::to_string(total_production_time_) +
        " | queued: " + queued_at_ +
        " | started: " + (started_at_.has_value() ? *started_at_ : std::string("null")) +
        " | completed: " + (completed_at_.has_value() ? *completed_at_ : std::string("null"));
}

ProductionQueueItem ProductionQueueItem::fromJson(JsonParser& parser) {
    ProductionQueueItem p;
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
            p.order_no_ = parser.parseString();
        }
        else if (key == "sample_id") {
            p.sample_id_ = parser.parseString();
        }
        else if (key == "shortage_qty") {
            p.shortage_qty_ = parser.parseNumber();
        }
        else if (key == "actual_production_qty") {
            p.actual_production_qty_ = parser.parseNumber();
        }
        else if (key == "total_production_time") {
            p.total_production_time_ = parser.parseDouble();
        }
        else if (key == "queued_at") {
            p.queued_at_ = parser.parseString();
        }
        else if (key == "started_at") {
            if (parser.isNull()) {
                parser.parseNull();
                p.started_at_ = std::nullopt;
            } else {
                p.started_at_ = parser.parseString();
            }
        }
        else if (key == "completed_at") {
            if (parser.isNull()) {
                parser.parseNull();
                p.completed_at_ = std::nullopt;
            } else {
                p.completed_at_ = parser.parseString();
            }
        }

        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
    parser.advance(); // '}'
    return p;
}
