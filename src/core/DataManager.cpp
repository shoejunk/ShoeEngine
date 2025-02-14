#include "DataManager.h"
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace ShoeEngine {
namespace Core {

DataManager::DataManager() = default;
DataManager::~DataManager() = default;

bool DataManager::RegisterManager(std::unique_ptr<BaseManager> manager) {
    auto type = manager->GetManagedType();
    if (m_managers.find(type) != m_managers.end()) {
        return false;
    }
    m_managers[type] = std::move(manager);
    return true;
}

Hash::HashValue DataManager::RegisterString(const std::string& str) {
    Hash::HashValue hash(str.c_str(), static_cast<uint32_t>(str.length()));
    m_stringRegistry[hash] = str;
    return hash;
}

const std::string& DataManager::GetString(const Hash::HashValue& hash) const {
    static const std::string empty;
    auto it = m_stringRegistry.find(hash);
    return it != m_stringRegistry.end() ? it->second : empty;
}

bool DataManager::LoadFromFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }

        nlohmann::json jsonData;
        file >> jsonData;
        return ProcessData(jsonData);
    }
    catch (const std::exception&) {
        return false;
    }
}

bool DataManager::ProcessData(const nlohmann::json& jsonData) {
    bool anyManagerProcessed = false;
    bool allProcessedSuccessfully = true;

	if (!jsonData.is_object()) {
		return false;
	}

	for (auto jit = jsonData.begin(); jit != jsonData.end(); ++jit) {
        Hash::HashValue typeHash(jit.key().c_str(), static_cast<uint32_t>(jit.key().length()));
            
        auto it = m_managers.find(typeHash);
        if (it != m_managers.end()) {
            anyManagerProcessed = true;
            if (!it->second->CreateFromJson(jit.value())) {
                allProcessedSuccessfully = false;
            }
        }
    }

    return anyManagerProcessed && allProcessedSuccessfully;
}

bool DataManager::SaveToFile(const std::string& filePath)
{
	try {
		// Create a top-level JSON object
		nlohmann::json rootJson = nlohmann::json::object();

		// Iterate through all managers
		for (const auto& [type, manager] : m_managers)
		{
			// Serialize the manager to JSON
			nlohmann::json managerData = manager->SerializeToJson();
			if (!managerData.empty())
			{
				// Use the type name (e.g. "sprites", "images") as a key in the root object
				const std::string typeName = GetString(type);
				rootJson[typeName] = managerData;
			}
		}

		// Ensure the directory exists
		std::filesystem::path pathObj(filePath);
		std::filesystem::path directory = pathObj.parent_path();
		if (!directory.empty() && !std::filesystem::exists(directory)) {
			// Create directories if they don't exist
			std::filesystem::create_directories(directory);
		}

		// Write the completed JSON to file
		std::ofstream file(filePath);
		if (!file.is_open()) {
			return false;
		}
		file << rootJson.dump(4); // Pretty-print with 4 spaces
		file.close();

		return true;
	}
	catch (const std::exception&) {
		return false;
	}
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
