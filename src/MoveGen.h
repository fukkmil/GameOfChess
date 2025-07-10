#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <vector>
#include "GameState.h"
#include "Move.h"
#include "Piece.h"

class MoveGenerator {
public:
    static std::vector<Move> generateLegal(const GameState& state);
    static bool isInCheck(const Board& board, Color color);

private:
    static std::vector<Move> generatePseudoLegal(const GameState& state);
    static bool isAttacked(const Board& board, int row, int col, Color attacker);
};

#endif //MOVEGEN_H
