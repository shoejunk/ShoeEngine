#pragma once

#include "core/BaseManager.h"
#include "core/Hash.h"
#include "Input.h"
#include <unordered_map>
#include <memory>
#include <vector>

namespace ShoeEngine {
	namespace Input {

		class InputManager : public Core::BaseManager {
		public:
			/**
			 * @brief Constructor
			 * @param dataManager Reference to the DataManager for string registration
			 */
			explicit InputManager(Core::DataManager& dataManager);

			/**
			 * @brief Creates input bindings from JSON configuration
			 * @param jsonData JSON data containing input contexts and bindings
			 * @return bool True if inputs were created successfully
			 */
			bool CreateFromJson(const nlohmann::json& jsonData) override;

			/**
			 * @brief Get the type of objects this manager handles
			 * @return Core::Hash::HashValue Returns hash of "input_contexts"
			 */
			Core::Hash::HashValue GetManagedType() const override;

			/**
			 * @brief Serialize all input contexts to JSON format
			 * @return nlohmann::json Structured input configuration
			 */
			nlohmann::json SerializeToJson() override;

			/**
			 * @brief Push a new context onto the active context stack
			 * @param contextName Name of the context to activate
			 */
			void PushContext(const std::string& contextName);

			/**
			 * @brief Pop the top context from the active context stack
			 */
			void PopContext();

			/**
			 * @brief Get an input binding from the current context stack
			 * @param name Hashed name of the input binding
			 * @return Input* Pointer to the input or nullptr if not found
			 */
			Input* GetInput(Core::Hash::HashValue name);

		private:
			struct InputContext {
				std::unordered_map<Core::Hash::HashValue, std::unique_ptr<Input>> inputs;
				Core::Hash::HashValue nameHash;
				std::string name;
			};

			std::unordered_map<Core::Hash::HashValue, InputContext> m_contexts;
			std::vector<Core::Hash::HashValue> m_activeContextStack;

			InputContext* GetOrCreateContext(const std::string& contextName);
			std::unique_ptr<Input> CreateInput(const nlohmann::json& inputData);
			sf::Keyboard::Key HashToKey(Core::Hash::HashValue keyHash) const;
			Core::Hash::HashValue KeyToHash(sf::Keyboard::Key key) const;
		};

	} // namespace Input
} // namespace ShoeEngine