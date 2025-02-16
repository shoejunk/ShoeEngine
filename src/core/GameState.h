#pragma once

#include <string>
#include <vector>
// Include any other standard headers you need

namespace ShoeEngine {
	namespace Core {

		/**
		 * @class GameState
		 * @brief Represents the pure data state of a game.
		 *
		 * This base class contains all data needed to describe the current
		 * state of the game, independent of any rendering or input systems.
		 * Derived classes should add game-specific data, such as scores,
		 * level layout, entity positions, etc.
		 *
		 * @note Since it is purely data, it should not include or depend on
		 *       any rendering, input, or other subsystems. Those systems
		 *       receive a GameState (or derived state) and decide how to
		 *       visualize or manipulate it.
		 */
		class GameState
		{
		public:
			/**
			 * @brief Virtual destructor for inheritance safety.
			 */
			virtual ~GameState() = default;

			/**
			 * @brief Optional method that derived classes can implement to reset state.
			 */
			virtual void ResetState() {}

			// -----------------------------------------------------------------------
			// Add any common methods/fields that all game states might need:
			// e.g. an ID, a state name, or a timestamp.
			// -----------------------------------------------------------------------

			// Example placeholder:
			// std::string stateName;
			// int currentLevel = 0;
		};

	} // namespace Core
} // namespace ShoeEngine
