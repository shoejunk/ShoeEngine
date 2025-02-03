#include <gtest/gtest.h>
#include "../../src/core/DataManager.h"
#include <memory>

using namespace ShoeEngine::Core;

// Mock manager for testing
class MockManager : public BaseManager {
public:
    MockManager(const std::string& type) : m_type(type) {}
    bool CreateFromJson(const nlohmann::json& jsonData) override {
        m_lastData = jsonData;
        return true;
    }
    std::string GetManagedType() const override { return m_type; }
    nlohmann::json GetLastData() const { return m_lastData; }

private:
    std::string m_type;
    nlohmann::json m_lastData;
};

class DataManagerTest : public ::testing::Test {
protected:
    DataManager dataManager;
};

TEST_F(DataManagerTest, RegisterManager) {
    auto manager = std::make_unique<MockManager>("test_type");
    EXPECT_TRUE(dataManager.RegisterManager(std::move(manager)));
}

TEST_F(DataManagerTest, RegisterDuplicateManager) {
    auto manager1 = std::make_unique<MockManager>("test_type");
    auto manager2 = std::make_unique<MockManager>("test_type");
    
    EXPECT_TRUE(dataManager.RegisterManager(std::move(manager1)));
    EXPECT_FALSE(dataManager.RegisterManager(std::move(manager2)));
}

TEST_F(DataManagerTest, ProcessValidData) {
    auto manager = std::make_unique<MockManager>("test_type");
    dataManager.RegisterManager(std::move(manager));

    nlohmann::json testData = {
        {"test_type", {{"key", "value"}}}
    };

    EXPECT_TRUE(dataManager.ProcessData(testData));
}
