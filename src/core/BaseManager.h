#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace ShoeEngine {
namespace Core {

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
     * @return std::string The type identifier for this manager
     */
    virtual std::string GetManagedType() const = 0;
};

} // namespace Core
} // namespace ShoeEngine
