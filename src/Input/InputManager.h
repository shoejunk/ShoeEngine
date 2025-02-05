#pragma once

#include "core/BaseManager.h"
#include "Input.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace ShoeEngine {
namespace Input {

/**
 * @class InputManager
 * @brief Manages input bindings and their associated actions
 *
 * This class handles the creation and management of Input objects from JSON data,
 * allowing for dynamic input configuration and context-based input handling.
 */
class InputManager : public Core::BaseManager {
public:
    /**
     * @brief Constructor for InputManager
     */
    InputManager();

    /**
     * @brief Create input bindings from JSON data
     * @param jsonData The JSON data containing input definitions
     * @return bool True if inputs were created successfully
     */
    bool CreateFromJson(const nlohmann::json& jsonData) override;

    /**
     * @brief Get the type of objects this manager handles
     * @return std::string Returns "inputs" as the managed type
     */
    std::string GetManagedType() const override { return "inputs"; }

    /**
     * @brief Update all input states
     */
    void Update();

    /**
     * @brief Set the current input context
     * @param context The context identifier
     */
    void SetContext(const std::string& context);

    /**
     * @brief Get an input binding by name
     * @param name The name of the input binding
     * @return Input* Pointer to the input binding, nullptr if not found
     */
    Input* GetInput(const std::string& name);

private:
    std::unordered_map<std::string, std::unique_ptr<Input>> m_inputs;
    std::string m_currentContext;

    /**
     * @brief Create an input binding from JSON data
     * @param inputData The JSON data for a single input binding
     * @return bool True if the input was created successfully
     */
    bool CreateInput(const nlohmann::json& inputData);
};

} // namespace Input
} // namespace ShoeEngine
