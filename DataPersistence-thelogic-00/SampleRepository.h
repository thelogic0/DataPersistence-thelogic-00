#pragma once

#include "Sample.h"
#include <string>
#include <vector>

class SampleRepository {
public:
    explicit SampleRepository(std::string filePath);

    const std::vector<Sample>& getAll() const;
    Sample* findById(const std::string& sampleId);

    bool add(const Sample& sample);
    bool remove(const std::string& sampleId);
    bool update(const Sample& sample);

    void save() const;

private:
    void load();

    std::string filePath_;
    std::vector<Sample> samples_;
};
