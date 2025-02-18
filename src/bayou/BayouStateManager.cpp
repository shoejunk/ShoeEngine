#include "BayouStateManager.h"
#include "core/Hash.h"
#include "core/DataManager.h"
#include <stdexcept>

namespace ShoeEngine {
	namespace Bayou {

		BayouStateManager::BayouStateManager(ShoeEngine::Core::DataManager& dataManager)
			: ShoeEngine::Core::BaseManager(dataManager)
		{
			// Register the manager type string.
			m_dataManager.RegisterString("bayou_state");
		}

		ShoeEngine::Core::Hash::HashValue BayouStateManager::GetManagedType() const {
			return "bayou_state"_h;
		}

		bool BayouStateManager::CreateFromJson(const nlohmann::json& jsonData) {
			try {
				// Expect JSON to have a "board" key (an array of 64 uint8_t values)
				// and a "pieces" key with "player1" and "player2" arrays.
				if (!jsonData.is_object()) {
					return false;
				}

				// Reset state before loading new data.
				m_state.ResetState();

				// Process board.
				if (jsonData.contains("board") && jsonData["board"].is_array()) {
					const auto& boardArray = jsonData["board"];
					if (boardArray.size() != BayouState::kBoardNumSquares) {
						throw std::runtime_error("Board array must have 64 elements.");
					}
					for (size_t i = 0; i < boardArray.size(); ++i) {
						m_state.m_board[i] = boardArray[i].get<uint8_t>();
					}
				}
				else {
					return false; // Missing board data.
				}

				// Process pieces.
				if (jsonData.contains("pieces") && jsonData["pieces"].is_object()) {
					const auto& piecesObj = jsonData["pieces"];

					// Player 1 pieces.
					if (piecesObj.contains("player1") && piecesObj["player1"].is_array()) {
						const auto& piecesArray = piecesObj["player1"];
						int count = 0;
						for (const auto& pieceJson : piecesArray) {
							if (!pieceJson.contains("type") || !pieceJson.contains("boardIndex"))
								continue;
							// Register and get the type hash.
							std::string typeStr = pieceJson["type"].get<std::string>();
							ShoeEngine::Core::Hash::HashValue typeHash = m_dataManager.RegisterString(typeStr);
							m_state.m_playerPieces[0][count].m_type = typeHash;
							m_state.m_playerPieces[0][count].m_boardIndex = pieceJson["boardIndex"].get<uint8_t>();
							++count;
						}
						m_state.m_numPieces[0] = count;
					}

					// Player 2 pieces.
					if (piecesObj.contains("player2") && piecesObj["player2"].is_array()) {
						const auto& piecesArray = piecesObj["player2"];
						int count = 0;
						for (const auto& pieceJson : piecesArray) {
							if (!pieceJson.contains("type") || !pieceJson.contains("boardIndex"))
								continue;
							std::string typeStr = pieceJson["type"].get<std::string>();
							ShoeEngine::Core::Hash::HashValue typeHash = m_dataManager.RegisterString(typeStr);
							m_state.m_playerPieces[1][count].m_type = typeHash;
							m_state.m_playerPieces[1][count].m_boardIndex = pieceJson["boardIndex"].get<uint8_t>();
							++count;
						}
						m_state.m_numPieces[1] = count;
					}
				}
				else {
					return false; // Missing pieces data.
				}

				return true;
			}
			catch (const std::exception&) {
				return false;
			}
		}

		nlohmann::json BayouStateManager::SerializeToJson() {
			nlohmann::json jsonData;

			// Serialize board.
			jsonData["board"] = nlohmann::json::array();
			for (const auto& cell : m_state.m_board) {
				jsonData["board"].push_back(cell);
			}

			// Serialize pieces.
			nlohmann::json piecesJson;

			// Serialize player 1 pieces.
			piecesJson["player1"] = nlohmann::json::array();
			for (int i = 0; i < m_state.m_numPieces[0]; ++i) {
				nlohmann::json pieceJson;
				std::string typeStr = m_dataManager.GetString(m_state.m_playerPieces[0][i].m_type);
				pieceJson["type"] = typeStr;
				pieceJson["boardIndex"] = m_state.m_playerPieces[0][i].m_boardIndex;
				piecesJson["player1"].push_back(pieceJson);
			}

			// Serialize player 2 pieces.
			piecesJson["player2"] = nlohmann::json::array();
			for (int i = 0; i < m_state.m_numPieces[1]; ++i) {
				nlohmann::json pieceJson;
				std::string typeStr = m_dataManager.GetString(m_state.m_playerPieces[1][i].m_type);
				pieceJson["type"] = typeStr;
				pieceJson["boardIndex"] = m_state.m_playerPieces[1][i].m_boardIndex;
				piecesJson["player2"].push_back(pieceJson);
			}

			jsonData["pieces"] = piecesJson;

			return jsonData;
		}

		BayouState& BayouStateManager::GetState() {
			return m_state;
		}

	} // namespace Bayou
} // namespace ShoeEngine
