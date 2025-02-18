#pragma once

#include "core/BaseManager.h"
#include "bayou/BayouStateManager.h"
#include "bayou/BayouState.h"
#include <nlohmann/json.hpp>

namespace ShoeEngine {
	namespace Bayou {

		class BayouStateManager : public ShoeEngine::Core::BaseManager {
		public:
			explicit BayouStateManager(ShoeEngine::Core::DataManager& dataManager);
			virtual ~BayouStateManager() = default;

			// Create the game state from JSON data.
			bool CreateFromJson(const nlohmann::json& jsonData) override;

			// Return the type identifier for this manager.
			ShoeEngine::Core::Hash::HashValue GetManagedType() const override;

			// Serialize the BayouState to JSON.
			nlohmann::json SerializeToJson() override;

			// Access the internal BayouState.
			BayouState& GetState();

		private:
			BayouState m_state;
		};

	} // namespace Bayou
} // namespace ShoeEngine
