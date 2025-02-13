#pragma once

#include "Hash.h"
#include <nlohmann/json.hpp>

namespace ShoeEngine {
namespace Core {

class DataManager;  // Forward declaration

/**
 * @class BaseManager
 * @brief Base class for all managers that handle object creation from JSON data
 *
 * This abstract class defines the interface that all specific managers must implement
 * to handle creation of their respective objects from JSON data.
 */
class BaseManager {
public:
    /**
     * @brief Constructor taking a reference to the DataManager
     * @param dataManager Reference to the DataManager for string registration
     */
    explicit BaseManager(DataManager& dataManager) : m_dataManager(dataManager) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~BaseManager() = default;

    /**
     * @brief Pure virtual function to handle object creation from JSON data
     * @param jsonData The JSON data containing object definitions
     * @return bool True if the objects were created successfully, false otherwise
     */
    virtual bool CreateFromJson(const nlohmann::json& jsonData) = 0;

    /**
     * @brief Get the type of objects this manager handles
     * @return Hash::HashValue The type identifier for this manager
     */
    virtual Hash::HashValue GetManagedType() const = 0;

    /**
     * @brief Virtual function to serialize managed objects to JSON
     * @return nlohmann::json JSON object containing serialized data of all managed objects
     */
    virtual nlohmann::json SerializeToJson() {
        return nlohmann::json::array();
    }

protected:
    DataManager& m_dataManager;  // Reference to the DataManager for string registration
};

} // namespace Core
} // namespace ShoeEngine
