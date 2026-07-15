#include "ConsoleApp.h"
#include <iostream>
#include <optional>

ConsoleApp::ConsoleApp(const std::string& sampleFile, const std::string& orderFile, const std::string& queueFile)
    : sampleRepository_(sampleFile), orderRepository_(orderFile), queueRepository_(queueFile) {}

std::string ConsoleApp::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int ConsoleApp::readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            return std::stoi(line);
        } catch (...) {
            std::cout << "숫자를 입력하세요." << std::endl;
        }
    }
}

double ConsoleApp::readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        try {
            return std::stod(line);
        } catch (...) {
            std::cout << "숫자를 입력하세요." << std::endl;
        }
    }
}

void ConsoleApp::printMainMenu() const {
    std::cout << "\n===== DataPersistence PoC =====" << std::endl;
    std::cout << "1. Sample CRUD" << std::endl;
    std::cout << "2. Order CRUD" << std::endl;
    std::cout << "3. ProductionQueueItem CRUD" << std::endl;
    std::cout << "0. 종료" << std::endl;
    std::cout << "선택: ";
}

void ConsoleApp::runSampleMenu() {
    while (true) {
        std::cout << "\n-- Sample CRUD --" << std::endl;
        std::cout << "1. Create  2. Read All  3. Read by ID  4. Update  5. Delete  0. 뒤로" << std::endl;
        std::string choice = readLine("선택: ");

        if (choice == "1") {
            std::string id = readLine("sample_id: ");
            if (sampleRepository_.findById(id) != nullptr) {
                std::cout << "이미 존재하는 sample_id 입니다." << std::endl;
                continue;
            }
            std::string name = readLine("name: ");
            double avgTime = readDouble("avg_production_time: ");
            double yieldRate = readDouble("yield_rate: ");
            int stock = readInt("current_stock: ");
            sampleRepository_.add(Sample(id, name, avgTime, yieldRate, stock));
            std::cout << "생성 완료 (" << id << ")" << std::endl;
        }
        else if (choice == "2") {
            const auto& all = sampleRepository_.getAll();
            if (all.empty()) {
                std::cout << "저장된 데이터가 없습니다." << std::endl;
            }
            for (const auto& s : all) {
                std::cout << s.toDisplayString() << std::endl;
            }
        }
        else if (choice == "3") {
            std::string id = readLine("sample_id: ");
            Sample* s = sampleRepository_.findById(id);
            if (!s) {
                std::cout << "찾을 수 없습니다." << std::endl;
            } else {
                std::cout << s->toDisplayString() << std::endl;
            }
        }
        else if (choice == "4") {
            std::string id = readLine("수정할 sample_id: ");
            Sample* existing = sampleRepository_.findById(id);
            if (!existing) {
                std::cout << "찾을 수 없습니다." << std::endl;
                continue;
            }
            std::string name = readLine("name (" + existing->getName() + "): ");
            double avgTime = readDouble("avg_production_time: ");
            double yieldRate = readDouble("yield_rate: ");
            int stock = readInt("current_stock: ");
            sampleRepository_.update(Sample(id, name, avgTime, yieldRate, stock));
            std::cout << "수정 완료" << std::endl;
        }
        else if (choice == "5") {
            std::string id = readLine("삭제할 sample_id: ");
            bool ok = sampleRepository_.remove(id);
            std::cout << (ok ? "삭제 완료" : "찾을 수 없습니다.") << std::endl;
        }
        else if (choice == "0") {
            return;
        }
        else {
            std::cout << "잘못된 선택입니다." << std::endl;
        }
    }
}

void ConsoleApp::runOrderMenu() {
    while (true) {
        std::cout << "\n-- Order CRUD --" << std::endl;
        std::cout << "1. Create  2. Read All  3. Read by ID  4. Update  5. Delete  0. 뒤로" << std::endl;
        std::string choice = readLine("선택: ");

        if (choice == "1") {
            std::string orderNo = readLine("order_no: ");
            if (orderRepository_.findById(orderNo) != nullptr) {
                std::cout << "이미 존재하는 order_no 입니다." << std::endl;
                continue;
            }
            std::string sampleId = readLine("sample_id: ");
            std::string customerName = readLine("customer_name: ");
            int quantity = readInt("quantity: ");
            std::string createdAt = readLine("created_at (예: 2026-07-15 10:00:00): ");
            orderRepository_.add(Order(orderNo, sampleId, customerName, quantity, "RESERVED", createdAt, createdAt));
            std::cout << "생성 완료 (" << orderNo << ")" << std::endl;
        }
        else if (choice == "2") {
            const auto& all = orderRepository_.getAll();
            if (all.empty()) {
                std::cout << "저장된 데이터가 없습니다." << std::endl;
            }
            for (const auto& o : all) {
                std::cout << o.toDisplayString() << std::endl;
            }
        }
        else if (choice == "3") {
            std::string orderNo = readLine("order_no: ");
            Order* o = orderRepository_.findById(orderNo);
            if (!o) {
                std::cout << "찾을 수 없습니다." << std::endl;
            } else {
                std::cout << o->toDisplayString() << std::endl;
            }
        }
        else if (choice == "4") {
            std::string orderNo = readLine("수정할 order_no: ");
            Order* existing = orderRepository_.findById(orderNo);
            if (!existing) {
                std::cout << "찾을 수 없습니다." << std::endl;
                continue;
            }
            std::string status = readLine("status (" + existing->getStatus() + "): ");
            std::string updatedAt = readLine("updated_at: ");
            Order updated(*existing);
            updated.setStatus(status);
            updated.setUpdatedAt(updatedAt);
            orderRepository_.update(updated);
            std::cout << "수정 완료" << std::endl;
        }
        else if (choice == "5") {
            std::string orderNo = readLine("삭제할 order_no: ");
            bool ok = orderRepository_.remove(orderNo);
            std::cout << (ok ? "삭제 완료" : "찾을 수 없습니다.") << std::endl;
        }
        else if (choice == "0") {
            return;
        }
        else {
            std::cout << "잘못된 선택입니다." << std::endl;
        }
    }
}

void ConsoleApp::runQueueMenu() {
    while (true) {
        std::cout << "\n-- ProductionQueueItem CRUD --" << std::endl;
        std::cout << "1. Create  2. Read All  3. Read by ID  4. Update  5. Delete  0. 뒤로" << std::endl;
        std::string choice = readLine("선택: ");

        if (choice == "1") {
            std::string orderNo = readLine("order_no: ");
            if (queueRepository_.findById(orderNo) != nullptr) {
                std::cout << "이미 존재하는 order_no 입니다." << std::endl;
                continue;
            }
            std::string sampleId = readLine("sample_id: ");
            int shortageQty = readInt("shortage_qty: ");
            int actualQty = readInt("actual_production_qty: ");
            double totalTime = readDouble("total_production_time(min): ");
            std::string queuedAt = readLine("queued_at: ");
            queueRepository_.add(ProductionQueueItem(orderNo, sampleId, shortageQty, actualQty, totalTime, queuedAt));
            std::cout << "생성 완료 (" << orderNo << ")" << std::endl;
        }
        else if (choice == "2") {
            const auto& all = queueRepository_.getAll();
            if (all.empty()) {
                std::cout << "저장된 데이터가 없습니다." << std::endl;
            }
            for (const auto& p : all) {
                std::cout << p.toDisplayString() << std::endl;
            }
        }
        else if (choice == "3") {
            std::string orderNo = readLine("order_no: ");
            ProductionQueueItem* p = queueRepository_.findById(orderNo);
            if (!p) {
                std::cout << "찾을 수 없습니다." << std::endl;
            } else {
                std::cout << p->toDisplayString() << std::endl;
            }
        }
        else if (choice == "4") {
            std::string orderNo = readLine("수정할 order_no: ");
            ProductionQueueItem* existing = queueRepository_.findById(orderNo);
            if (!existing) {
                std::cout << "찾을 수 없습니다." << std::endl;
                continue;
            }
            std::string started = readLine("started_at (없으면 빈 값 입력): ");
            std::string completed = readLine("completed_at (없으면 빈 값 입력): ");
            ProductionQueueItem updated(*existing);
            updated.setStartedAt(started.empty() ? std::nullopt : std::optional<std::string>(started));
            updated.setCompletedAt(completed.empty() ? std::nullopt : std::optional<std::string>(completed));
            queueRepository_.update(updated);
            std::cout << "수정 완료" << std::endl;
        }
        else if (choice == "5") {
            std::string orderNo = readLine("삭제할 order_no: ");
            bool ok = queueRepository_.remove(orderNo);
            std::cout << (ok ? "삭제 완료" : "찾을 수 없습니다.") << std::endl;
        }
        else if (choice == "0") {
            return;
        }
        else {
            std::cout << "잘못된 선택입니다." << std::endl;
        }
    }
}

void ConsoleApp::run() {
    while (true) {
        printMainMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            runSampleMenu();
        }
        else if (choice == "2") {
            runOrderMenu();
        }
        else if (choice == "3") {
            runQueueMenu();
        }
        else if (choice == "0") {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }
        else {
            std::cout << "잘못된 선택입니다. 다시 입력해주세요." << std::endl;
        }
    }
}
