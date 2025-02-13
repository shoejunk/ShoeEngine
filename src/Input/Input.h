#pragma once

#include "core/Hash.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <unordered_map>

namespace ShoeEngine {
	namespace Input {

		class Input {
		public:
			enum class Type {
				Keyboard,
				MouseButton,
				MouseAxis
			};

			explicit Input(Type type) : m_type(type) {}
			virtual ~Input() = default;

			virtual void Update() = 0;
			virtual bool IsActive() const = 0;

			Type GetType() const { return m_type; }
			Core::Hash::HashValue GetName() const { return m_name; }
			void SetContext(Core::Hash::HashValue context) { m_context = context; }
			Core::Hash::HashValue GetContext() const { return m_context; }

		protected:
			Type m_type;
			Core::Hash::HashValue m_name;
			Core::Hash::HashValue m_context;
		};

		class KeyboardInput : public Input {
		public:
			explicit KeyboardInput(Core::Hash::HashValue name)
				: Input(Type::Keyboard), m_key(sf::Keyboard::Unknown) {
				m_name = name;
			}

			void SetKey(sf::Keyboard::Key key) { m_key = key; }
			sf::Keyboard::Key GetKey() const { return m_key; }
			void Update() override;
			bool IsActive() const override;

		private:
			sf::Keyboard::Key m_key;
		};

		class MouseButtonInput : public Input {
		public:
			explicit MouseButtonInput(Core::Hash::HashValue name)
				: Input(Type::MouseButton), m_button(sf::Mouse::ButtonCount) {
				m_name = name;
			}

			void SetButton(sf::Mouse::Button button) { m_button = button; }
			sf::Mouse::Button GetButton() const { return m_button; }
			void Update() override;
			bool IsActive() const override;

		private:
			sf::Mouse::Button m_button;
		};

		class MouseAxisInput : public Input {
		public:
			MouseAxisInput(Core::Hash::HashValue name, bool isXAxis)
				: Input(Type::MouseAxis), m_isXAxis(isXAxis) {
				m_name = name;
			}

			bool IsXAxis() const { return m_isXAxis; }
			void Update() override;
			bool IsActive() const override;

		private:
			bool m_isXAxis;
			mutable sf::Vector2i m_lastMousePos;
		};

	} // namespace Input
} // namespace ShoeEngine