#include "Input/Input.h"
#include <SFML/Window/Mouse.hpp>

namespace ShoeEngine {
	namespace Input {

		void MouseButtonInput::Update() {
			// State updated during IsActive check
		}

		bool MouseButtonInput::IsActive() const {
			return sf::Mouse::isButtonPressed(m_button);
		}

	} // namespace Input
} // namespace ShoeEngine