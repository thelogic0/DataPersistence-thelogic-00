#ifdef _DEBUG

#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else

#include "ConsoleApp.h"

int main() {
    ConsoleApp app("samples.json", "orders.json", "production_queue.json");
    app.run();
    return 0;
}

#endif
