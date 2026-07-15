#include "SampleRepository.h"
#include "Json.h"
#include <algorithm>
#include <fstream>
#include <sstream>

SampleRepository::SampleRepository(std::string filePath) : filePath_(std::move(filePath)) {
    load();
}

void SampleRepository::load() {
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
            samples_.push_back(Sample::fromJson(parser));
        }
        parser.skipWhitespace();
        if (!parser.isEnd() && parser.peek() == ',') {
            parser.advance();
        }
    }
}

void SampleRepository::save() const {
    std::ofstream out(filePath_);
    out << "[\n";
    for (size_t i = 0; i < samples_.size(); ++i) {
        out << samples_[i].toJson();
        out << (i + 1 < samples_.size() ? ",\n" : "\n");
    }
    out << "]\n";
}

const std::vector<Sample>& SampleRepository::getAll() const {
    return samples_;
}

Sample* SampleRepository::findById(const std::string& sampleId) {
    auto it = std::find_if(samples_.begin(), samples_.end(),
        [&sampleId](const Sample& s) { return s.getSampleId() == sampleId; });
    return it == samples_.end() ? nullptr : &(*it);
}

bool SampleRepository::add(const Sample& sample) {
    if (findById(sample.getSampleId()) != nullptr) {
        return false;
    }
    samples_.push_back(sample);
    save();
    return true;
}

bool SampleRepository::remove(const std::string& sampleId) {
    auto it = std::find_if(samples_.begin(), samples_.end(),
        [&sampleId](const Sample& s) { return s.getSampleId() == sampleId; });
    if (it == samples_.end()) {
        return false;
    }
    samples_.erase(it);
    save();
    return true;
}

bool SampleRepository::update(const Sample& sample) {
    Sample* existing = findById(sample.getSampleId());
    if (!existing) {
        return false;
    }
    *existing = sample;
    save();
    return true;
}
