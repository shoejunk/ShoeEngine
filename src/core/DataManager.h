#pragma once

#include "BaseManager.h"
#include "Hash.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace ShoeEngine {
namespace Core {

/**
 * @class DataManager
 * @brief Central manager for loading and distributing JSON data to type-specific managers
 *
 * This class is responsible for reading JSON data files and distributing the data
 * to the appropriate managers based on object types.
 */
class DataManager {
public:
    /**
     * @brief Constructor
     */
    DataManager();

    /**
     * @brief Destructor
     */
    ~DataManager();

    /**
     * @brief Register a manager for a specific object type
     * @param manager Pointer to the manager to register
     * @return bool True if registration was successful, false if type already registered
     */
    bool RegisterManager(std::unique_ptr<BaseManager> manager);

    /**
     * @brief Load and process JSON data from a file
     * @param filePath Path to the JSON file
     * @return bool True if loading and processing was successful
     */
    bool LoadFromFile(const std::string& filePath);

    /**
     * @brief Process JSON data directly
     * @param jsonData The JSON data to process
     * @return bool True if processing was successful
     */
    bool ProcessData(const nlohmann::json& jsonData);

    /**
     * @brief Get a registered manager by its type
     * @param type The type of manager to retrieve
     * @return BaseManager* Pointer to the manager, or nullptr if not found
     */
    BaseManager* GetManager(const Core::Hash::HashValue& type);

private:
    std::unordered_map<Core::Hash::HashValue, std::unique_ptr<BaseManager>, Core::Hash::Hasher> m_managers;
};

} // namespace Core
} // namespace ShoeEngine
