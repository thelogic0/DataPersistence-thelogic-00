#pragma once

#include "OrderRepository.h"
#include "ProductionQueueRepository.h"
#include "SampleRepository.h"
#include <string>

class ConsoleApp {
public:
    ConsoleApp(const std::string& sampleFile, const std::string& orderFile, const std::string& queueFile);

    void run();

private:
    static std::string readLine(const std::string& prompt);
    static int readInt(const std::string& prompt);
    static double readDouble(const std::string& prompt);

    void printMainMenu() const;
    void runSampleMenu();
    void runOrderMenu();
    void runQueueMenu();

    SampleRepository sampleRepository_;
    OrderRepository orderRepository_;
    ProductionQueueRepository queueRepository_;
};
