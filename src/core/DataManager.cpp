#include "DataManager.h"
#include <fstream>
#include <iostream>

namespace ShoeEngine {
namespace Core {

DataManager::DataManager() {}
DataManager::~DataManager() {}

bool DataManager::RegisterManager(std::unique_ptr<BaseManager> manager) {
    if (!manager) {
        return false;
    }

    Core::Hash::HashValue type = manager->GetManagedType();
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
    bool anyManagerProcessed = false;
    bool allProcessedSuccessfully = true;

    for (const auto& [type, data] : jsonData.items()) {
        Core::Hash::HashValue typeHash(type.c_str(), type.length());
        auto managerIt = m_managers.find(typeHash);
        if (managerIt != m_managers.end()) {
            if (!managerIt->second->CreateFromJson(data)) {
                std::cerr << "Failed to process data for type: " << type << std::endl;
                allProcessedSuccessfully = false;
            }
            anyManagerProcessed = true;
        }
        else {
            // Just log that no manager is registered for this type
            std::cout << "No manager registered for type: " << type << std::endl;
        }
    }

    // Return true if at least one manager processed its data successfully
    return anyManagerProcessed && allProcessedSuccessfully;
}

BaseManager* DataManager::GetManager(const Core::Hash::HashValue& type) {
    auto it = m_managers.find(type);
    if (it != m_managers.end()) {
        return it->second.get();
    }
    return nullptr;
}

} // namespace Core
} // namespace ShoeEngine
