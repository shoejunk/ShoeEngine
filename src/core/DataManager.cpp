#include "DataManager.h"
#include <fstream>
#include <iostream>

namespace ShoeEngine {
namespace Core {

DataManager::DataManager() = default;
DataManager::~DataManager() = default;

bool DataManager::RegisterManager(std::unique_ptr<BaseManager> manager) {
    if (!manager) {
        return false;
    }

    std::string type = manager->GetManagedType();
    if (m_managers.find(type) != m_managers.end()) {
        return false;
    }

    m_managers[type] = std::move(manager);
    return true;
}

bool DataManager::LoadFromFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return false;
        }

        nlohmann::json jsonData;
        file >> jsonData;
        return ProcessData(jsonData);
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading JSON file: " << e.what() << std::endl;
        return false;
    }
}

bool DataManager::ProcessData(const nlohmann::json& jsonData) {
    try {
        bool success = true;
        for (const auto& [type, data] : jsonData.items()) {
            auto managerIt = m_managers.find(type);
            if (managerIt != m_managers.end()) {
                if (!managerIt->second->CreateFromJson(data)) {
                    std::cerr << "Failed to process data for type: " << type << std::endl;
                    success = false;
                }
            }
            else {
                std::cerr << "No manager registered for type: " << type << std::endl;
                success = false;
            }
        }
        return success;
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing JSON data: " << e.what() << std::endl;
        return false;
    }
}

} // namespace Core
} // namespace ShoeEngine
