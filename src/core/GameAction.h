#pragma once

#include "core/GameState.h"

namespace ShoeEngine {
	namespace Core {

		/**
		 * @class GameAction
		 * @brief Base class for any action that can be applied to a GameState.
		 *
		 * A GameAction attempts to modify a GameState. If the action is legal
		 * in the context of that state, the action applies its changes and
		 * returns true. If it's illegal, the action leaves the GameState
		 * unmodified and returns false.
		 *
		 * Derived classes are expected to override Apply() with specific
		 * logic determining legality and performing modifications.
		 */
		class GameAction
		{
		public:
			/**
			 * @brief Virtual destructor for inheritance safety.
			 */
			virtual ~GameAction() = default;

			/**
			 * @brief Attempt to apply this action to the given GameState.
			 * @param state Reference to the GameState to be modified.
			 * @return true if the action was applied successfully, false if it was illegal.
			 *
			 * @note Implementations should first check if the action can be applied
			 *       without violating game rules. If valid, modify the state and
			 *       return true; otherwise, do not modify the state and return false.
			 */
			virtual bool Apply(GameState& state) = 0;
		};

	} // namespace Core
} // namespace ShoeEngine
