#include "Sample.h"

Sample::Sample(std::string sampleId, std::string name, double avgProductionTime, double yieldRate, int currentStock)
    : sample_id_(std::move(sampleId)), name_(std::move(name)),
      avg_production_time_(avgProductionTime), yield_rate_(yieldRate), current_stock_(currentStock) {}

const std::string& Sample::getSampleId() const { return sample_id_; }
const std::string& Sample::getName() const { return name_; }
double Sample::getAvgProductionTime() const { return avg_production_time_; }
double Sample::getYieldRate() const { return yield_rate_; }
int Sample::getCurrentStock() const { return current_stock_; }

void Sample::setName(const std::string& value) { name_ = value; }
void Sample::setAvgProductionTime(double value) { avg_production_time_ = value; }
void Sample::setYieldRate(double value) { yield_rate_ = value; }
void Sample::setCurrentStock(int value) { current_stock_ = value; }

std::string Sample::toJson() const {
    std::string out;
    out += "  {\n";
    out += "    \"sample_id\": \"" + JsonParser::escape(sample_id_) + "\",\n";
    out += "    \"name\": \"" + JsonParser::escape(name_) + "\",\n";
    out += "    \"avg_production_time\": " + std::to_string(avg_production_time_) + ",\n";
    out += "    \"yield_rate\": " + std::to_string(yield_rate_) + ",\n";
    out += "    \"current_stock\": " + std::to_string(current_stock_) + "\n";
    out += "  }";
    return out;
}

std::string Sample::toDisplayString() const {
    return "ID: " + sample_id_ +
        " | 이름: " + name_ +
        " | 평균생산시간: " + std::to_string(avg_production_time_) +
        " | 수율: " + std::to_string(yield_rate_) +
        " | 재고: " + std::to_string(current_stock_);
}

Sample Sample::fromJson(JsonParser& parser) {
    Sample s;
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

        if (key == "sample_id") {
            s.sample_id_ = parser.parseString();
        }
        else if (key == "name") {
            s.name_ = parser.parseString();
        }
        else if (key == "avg_production_time") {
            s.avg_production_time_ = parser.parseDouble();
        }
        else if (key == "yield_rate") {
            s.yield_rate_ = parser.parseDouble();
        }
        else if (key == "current_stock") {
            s.current_stock_ = parser.parseNumber();
        }

        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
    parser.advance(); // '}'
    return s;
}
