#include "BayouState.h"
#include <algorithm> // if needed

namespace ShoeEngine {
namespace Bayou {

// -------------------------------------------------------------------------
// Constructor
// -------------------------------------------------------------------------
BayouState::BayouState()
{
    // Initialize all squares as empty (0 => not occupied)
    for (auto &cell : m_board) {
        cell = 0;
    }
    // Initialize piece counts
    m_numPieces[0] = 0;
    m_numPieces[1] = 0;
}

// -------------------------------------------------------------------------
// ResetState
// -------------------------------------------------------------------------
void BayouState::ResetState()
{
    // Clear board
    for (auto &cell : m_board) {
        cell = 0;
    }
    // Reset each player's piece arrays
    for (int player = 0; player < 2; ++player) {
        m_numPieces[player] = 0;
        // Optionally clear data in each Piece if desired:
        // for (auto &p : m_playerPieces[player]) {
        //     p.m_type      = ShoeEngine::Core::Hash::HashValue{};
        //     p.m_boardIndex = 0;
        // }
    }
}

// -------------------------------------------------------------------------
// Static Board Utilities
// -------------------------------------------------------------------------
bool BayouState::IsOccupied(uint8_t cell)
{
    // bit 0 => Occupied
    return (cell & 0x01) != 0;
}

int BayouState::GetPlayerId(uint8_t cell)
{
    // bit 1 => which player
    return (cell & 0x02) ? 1 : 0;
}

int BayouState::GetPieceIndex(uint8_t cell)
{
    // bits 2..7 => piece index
    return static_cast<int>((cell & 0xFC) >> 2);
}

uint8_t BayouState::EncodeOccupied(int playerId, int pieceIdx)
{
    // bit0 => occupied
    // bit1 => player
    // bits2..7 => piece index
    uint8_t cell = 0x01; // mark as occupied
    if (playerId == 1) {
        cell |= 0x02; // set bit1 for player2
    }
    cell |= static_cast<uint8_t>((pieceIdx & 0x3F) << 2);
    return cell;
}

int BayouState::ToIndex(int row, int col)
{
    return row * 8 + col;
}

bool BayouState::IsOutOfBounds(int row, int col)
{
    return (row < 0 || row >= 8 || col < 0 || col >= 8);
}

// -------------------------------------------------------------------------
// PlaceNewPiece
// -------------------------------------------------------------------------
bool BayouState::PlaceNewPiece(int row, int col, int playerId, ShoeEngine::Core::Hash::HashValue pieceType)
{
    if (IsOutOfBounds(row, col)) {
        return false;
    }
    int idx = ToIndex(row, col);
    if (IsOccupied(m_board[idx])) {
        // Square already taken
        return false;
    }
    // Check capacity
    int &count = m_numPieces[playerId];
    if (count >= 64) {
        return false; // No more space
    }

    // Initialize the piece in the array
    int pieceArrayIndex = count;
    Piece &p = m_playerPieces[playerId][pieceArrayIndex];
    p.m_type       = pieceType;
    p.m_boardIndex = static_cast<uint8_t>(idx);

    // Mark the board cell
    m_board[idx] = EncodeOccupied(playerId, pieceArrayIndex);

    count++;
    return true;
}

// -------------------------------------------------------------------------
// RemovePiece
// -------------------------------------------------------------------------
bool BayouState::RemovePiece(int row, int col)
{
    if (IsOutOfBounds(row, col)) {
        return false;
    }
    int idx = ToIndex(row, col);
    uint8_t cell = m_board[idx];
    if (!IsOccupied(cell)) {
        // Already empty
        return false;
    }

    // Decode occupant
    int playerId   = GetPlayerId(cell);
    int pieceIndex = GetPieceIndex(cell);

    int &count     = m_numPieces[playerId];
    int lastIndex  = count - 1; // The last used piece index in that player's array

    // If the piece to remove isn't already the last piece, swap it
    if (pieceIndex != lastIndex)
    {
        // Move the last piece into pieceIndex
        m_playerPieces[playerId][pieceIndex] = m_playerPieces[playerId][lastIndex];

        // Update the board index for the piece we just moved
        Piece &movedPiece = m_playerPieces[playerId][pieceIndex];
        uint8_t oldCell   = m_board[movedPiece.m_boardIndex];
        // Re-encode occupant to use the new pieceIndex
        m_board[movedPiece.m_boardIndex] = EncodeOccupied(playerId, pieceIndex);
    }

    // Decrement the piece count
    count--;

    // Clear the square
    m_board[idx] = 0;
    return true;
}

} // namespace Bayou
} // namespace ShoeEngine

