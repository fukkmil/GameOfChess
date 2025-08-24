#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include <vector>
#include <optional>
#include <utility>
#include <map>
#include <qstring.h>
#include <string>

class GameState {
public:
    GameState();

    [[nodiscard]] const Board& board() const noexcept;
    [[nodiscard]] Color sideToMove() const noexcept;
    [[nodiscard]] bool playingEngine() const noexcept;
    [[nodiscard]] Color engineSide() const noexcept;
    [[nodiscard]] bool canCastleKingSide(Color color) const noexcept;
    [[nodiscard]] bool canCastleQueenSide(Color color) const noexcept;
    [[nodiscard]] std::optional<std::pair<int,int>> enPassantTarget() const noexcept;
    [[nodiscard]] int halfmoveClock() const noexcept;
    [[nodiscard]] const std::vector<Move>& history() const noexcept;
    void setPlayingEngine(bool enabled, bool engineIsWhite);
    [[nodiscard]] int repetitionCount() const noexcept;
    [[nodiscard]] std::string fenFull() const;


    void applyMove(const Move& move);

    bool undoMove();

private:
    Board board_;
    Color sideToMove_;
    bool playingEngine_;
    Color engineSide_;
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

};

#endif //GAMESTATE_H
