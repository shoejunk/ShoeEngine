#pragma once

#include "core/Hash.h"
#include <cstdint> // for uint8_t

namespace ShoeEngine {
namespace Bayou {

/**
 * @class Piece
 * @brief Represents a single piece owned by a player in Bayou Bonanza.
 *
 * Pieces store:
 *  - A 6-bit index referencing which entry they occupy in a player's piece array
 *  - A single byte (m_boardIndex) referencing which cell in the BayouState::m_board this piece occupies
 *  - A hash value representing the 'type' of piece (e.g. "Rook", "Gator", "Mosquito", etc.)
 */
class Piece
{
public:
    // The type of piece, stored as a hash (Core::Hash::HashValue).
    ShoeEngine::Core::Hash::HashValue m_type;

    /**
     * @brief The index into BayouState::m_board that this piece is currently occupying.
     *
     * This is a number 0..63, corresponding to an 8×8 board, row-major order.
     * Example:
     *  - row = 3, col = 5 => index = 3*8 + 5 = 29
     */
    uint8_t m_boardIndex = 0;
};

} // namespace Bayou
} // namespace ShoeEngine
