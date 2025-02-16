#pragma once

#include "core/GameState.h"
#include "core/Hash.h"
#include "Piece.h"

#include <array>
#include <cstdint>

namespace ShoeEngine {
namespace Bayou {

/**
 * @class BayouState
 * @brief A GameState for an 8x8 board game, supporting two players.
 *
 * Each of the 64 board squares is an 8-bit cell with:
 *   - bit 0: Occupied (1) or empty (0)
 *   - bit 1: Which player (0 for player1, 1 for player2)
 *   - bits 2..7: 6-bit piece index (0..63) referencing that player's pieces array
 */
class BayouState : public ShoeEngine::Core::GameState
{
public:
    // Board is always 8x8 => 64 squares
    static constexpr int kBoardNumSquares = 64;

    // ---------------------------------------------------------------------
    // Constructors / Destructor
    // ---------------------------------------------------------------------
    BayouState();
    virtual ~BayouState() = default; // Virtual destructor from base class

    // ---------------------------------------------------------------------
    // Board Representation
    // ---------------------------------------------------------------------
    /**
     * @brief An array of 64 bytes, each representing one cell on the board.
     */
    std::array<uint8_t, kBoardNumSquares> m_board{};

    /**
     * @brief Two arrays of up to 64 pieces each, one array per player.
     */
    std::array<Piece, 64> m_playerPieces[2];

    /**
     * @brief Tracks how many pieces each player currently uses in m_playerPieces.
     */
    int m_numPieces[2] = {0, 0};

    // ---------------------------------------------------------------------
    // Override from GameState
    // ---------------------------------------------------------------------
    void ResetState() override;

    // ---------------------------------------------------------------------
    // Board Utilities
    // ---------------------------------------------------------------------
    static bool IsOccupied(uint8_t cell);
    static int  GetPlayerId(uint8_t cell);
    static int  GetPieceIndex(uint8_t cell);
    static uint8_t EncodeOccupied(int playerId, int pieceIdx);

    static int  ToIndex(int row, int col);
    static bool IsOutOfBounds(int row, int col);

    // ---------------------------------------------------------------------
    // Gameplay Methods
    // ---------------------------------------------------------------------
    bool PlaceNewPiece(int row, int col, int playerId, ShoeEngine::Core::Hash::HashValue pieceType);
    bool RemovePiece(int row, int col);
};

} // namespace Bayou
} // namespace ShoeEngine
