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
 * to the appropriate managers based on object types. It also maintains a registry
 * of strings used by managers for serialization.
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
     * @brief Save all managed objects to a JSON file
     * @param filePath Path to save the JSON file
     * @return bool True if saving was successful
     */
    bool SaveToFile(const std::string& filePath);

    /**
     * @brief Get a registered manager by its type
     * @param type The type of manager to retrieve
     * @return BaseManager* Pointer to the manager, or nullptr if not found
     */
    BaseManager* GetManager(const Core::Hash::HashValue& type);

    /**
     * @brief Register a string with the DataManager and get its hash
     * @param str The string to register
     * @return HashValue The hash of the registered string
     */
    Hash::HashValue RegisterString(const std::string& str);

    /**
     * @brief Get the original string for a hash value
     * @param hash The hash value to look up
     * @return const std::string& The original string, or empty string if not found
     */
    const std::string& GetString(const Hash::HashValue& hash) const;

private:
    std::unordered_map<Hash::HashValue, std::unique_ptr<BaseManager>, Hash::Hasher> m_managers;
    std::unordered_map<Hash::HashValue, std::string, Hash::Hasher> m_stringRegistry;
};

} // namespace Core
} // namespace ShoeEngine
