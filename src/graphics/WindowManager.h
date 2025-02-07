#pragma once

#include "Window.h"
#include "../core/BaseManager.h"
#include "../core/Hash.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include <string>

namespace ShoeEngine {
namespace Graphics {

/**
 * @class WindowManager
 * @brief Manages creation and lifecycle of Window objects from JSON configuration
 *
 * The WindowManager class handles the creation of Window instances based on
 * JSON configuration data. It inherits from BaseManager to integrate with
 * the DataManager system.
 */
class WindowManager : public Core::BaseManager {
public:
    /**
     * @brief Constructor
     */
    WindowManager() = default;

    /**
     * @brief Destructor
     */
    ~WindowManager() override = default;

    /**
     * @brief Creates Window objects from JSON configuration
     * @param jsonData JSON data containing window configurations
     * @return bool True if windows were created successfully
     */
    bool CreateFromJson(const nlohmann::json& jsonData) override;

    /**
     * @brief Returns the type of objects this manager handles
     * @return Core::Hash::HashValue Hash of "windows"
     */
    Core::Hash::HashValue GetManagedType() const override;

    /**
     * @brief Process events for all managed windows
     * @return bool True if any windows are still open
     */
    bool ProcessEvents();

    /**
     * @brief Clear all managed windows
     */
    void ClearAll();

    /**
     * @brief Display all managed windows
     */
    void DisplayAll();

    /**
     * @brief Get all managed windows
     * @return const std::vector<std::unique_ptr<Window>>& Reference to the windows vector
     */
    const std::vector<std::unique_ptr<Window>>& GetWindows() const;

private:
    std::vector<std::unique_ptr<Window>> m_windows;  ///< Collection of managed windows
};

} // namespace Graphics
} // namespace ShoeEngine
