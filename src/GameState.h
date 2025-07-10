#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include <vector>
#include <optional>
#include <utility>
#include <map>
#include <string>

class GameState {
public:
    GameState();

    const Board& board() const noexcept;
    Color sideToMove() const noexcept;
    bool canCastleKingSide(Color color) const noexcept;
    bool canCastleQueenSide(Color color) const noexcept;
    std::optional<std::pair<int,int>> enPassantTarget() const noexcept;
    int halfmoveClock() const noexcept;
    const std::vector<Move>& history() const noexcept;

    int repetitionCount() const noexcept;

    void applyMove(const Move& move);

    bool undoMove();

private:
    Board board_;
    Color sideToMove_;
    bool whiteKingSideCastle_;
    bool whiteQueenSideCastle_;
    bool blackKingSideCastle_;
    bool blackQueenSideCastle_;
    std::optional<std::pair<int,int>> enPassantTarget_;
    int halfmoveClock_;
    std::vector<Move> history_;

    std::map<std::string,int> repetitionCounts_;

    struct StateSnapshot {
        Board board;
        Color side;
        bool wKS, wQS, bKS, bQS;
        std::optional<std::pair<int,int>> enPassant;
        int halfmoveClock;
        std::vector<Move> history;
        std::map<std::string,int> repetitionCounts;
    };
    std::vector<StateSnapshot> snapshots_;

    std::string positionKey() const;
};

#endif //GAMESTATE_H
