#include "BoardRenderer.h"

namespace ShoeEngine {
	namespace Bayou {

		BoardRenderer::BoardRenderer(int tileSize)
			: m_tileSize(tileSize)
		{
			// No texture loading since we're only drawing grid lines.
		}

		void BoardRenderer::Render(ShoeEngine::Graphics::Window& window) {
			const int gridCount = 8; // 8 columns and 8 rows.
			sf::VertexArray gridLines(sf::Lines);

			// Draw vertical grid lines.
			for (int i = 0; i <= gridCount; ++i) {
				float x = static_cast<float>(i * m_tileSize);
				gridLines.append(sf::Vertex(sf::Vector2f(x, 0.0f), sf::Color::White));
				gridLines.append(sf::Vertex(sf::Vector2f(x, static_cast<float>(gridCount * m_tileSize)), sf::Color::White));
			}

			// Draw horizontal grid lines.
			for (int i = 0; i <= gridCount; ++i) {
				float y = static_cast<float>(i * m_tileSize);
				gridLines.append(sf::Vertex(sf::Vector2f(0.0f, y), sf::Color::White));
				gridLines.append(sf::Vertex(sf::Vector2f(static_cast<float>(gridCount * m_tileSize), y), sf::Color::White));
			}

			window.GetRenderWindow().draw(gridLines);
		}

		bool BoardRenderer::GetBoardCell(float x, float y, int& row, int& col) const {
			const int gridCount = 8;
			if (x < 0 || y < 0 || x >= static_cast<float>(gridCount * m_tileSize) || y >= static_cast<float>(gridCount * m_tileSize)) {
				return false;
			}
			col = static_cast<int>(x) / m_tileSize;
			row = static_cast<int>(y) / m_tileSize;
			return true;
		}

	} // namespace Bayou
} // namespace ShoeEngine
