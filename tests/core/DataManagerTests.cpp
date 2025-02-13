#include <gtest/gtest.h>
#include "../../src/core/DataManager.h"
#include "../../src/core/Hash.h"
#include <memory>

using namespace ShoeEngine::Core;

// Mock manager for testing
class MockManager : public BaseManager {
public:
    MockManager(DataManager& dataManager, Hash::HashValue type) 
        : BaseManager(dataManager)
        , m_type(type) {}

    bool CreateFromJson(const nlohmann::json& jsonData) override {
        m_lastData = jsonData;
        return true;
    }
    Hash::HashValue GetManagedType() const override { return m_type; }
    nlohmann::json GetLastData() const { return m_lastData; }

private:
    Hash::HashValue m_type;
    nlohmann::json m_lastData;
};

class DataManagerTest : public ::testing::Test {
protected:
    DataManager dataManager;
};

TEST_F(DataManagerTest, RegisterManager) {
    auto manager = std::make_unique<MockManager>(dataManager, "test_type"_h);
    EXPECT_TRUE(dataManager.RegisterManager(std::move(manager)));
}

TEST_F(DataManagerTest, RegisterDuplicateManager) {
    auto manager1 = std::make_unique<MockManager>(dataManager, "test_type"_h);
    auto manager2 = std::make_unique<MockManager>(dataManager, "test_type"_h);
    
    EXPECT_TRUE(dataManager.RegisterManager(std::move(manager1)));
    EXPECT_FALSE(dataManager.RegisterManager(std::move(manager2)));
}

TEST_F(DataManagerTest, ProcessValidData) {
    auto manager = std::make_unique<MockManager>(dataManager, "test_type"_h);
    dataManager.RegisterManager(std::move(manager));

    nlohmann::json testData = {
        {"test_type", {{"key", "value"}}}
    };

    EXPECT_TRUE(dataManager.ProcessData(testData));
}

TEST_F(DataManagerTest, ProcessInvalidData) {
    auto manager = std::make_unique<MockManager>(dataManager, "test_type"_h);
    dataManager.RegisterManager(std::move(manager));

    nlohmann::json testData = {
        {"invalid_type", {{"key", "value"}}}
    };

    EXPECT_FALSE(dataManager.ProcessData(testData));
}

TEST_F(DataManagerTest, RegisterString) {
    std::string testString = "test_string";
    auto hash = dataManager.RegisterString(testString);
    EXPECT_EQ(dataManager.GetString(hash), testString);
}

TEST_F(DataManagerTest, GetNonexistentString) {
    Hash::HashValue hash("nonexistent"_h);
    EXPECT_TRUE(dataManager.GetString(hash).empty());
}

TEST_F(DataManagerTest, RegisterMultipleStrings) {
    std::string str1 = "string1";
    std::string str2 = "string2";
    
    auto hash1 = dataManager.RegisterString(str1);
    auto hash2 = dataManager.RegisterString(str2);
    
    EXPECT_EQ(dataManager.GetString(hash1), str1);
    EXPECT_EQ(dataManager.GetString(hash2), str2);
}

TEST_F(DataManagerTest, RegisterDuplicateString) {
    std::string testString = "test_string";
    auto hash1 = dataManager.RegisterString(testString);
    auto hash2 = dataManager.RegisterString(testString);
    
    EXPECT_EQ(hash1, hash2);
    EXPECT_EQ(dataManager.GetString(hash1), testString);
}
