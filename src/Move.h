#ifndef MOVE_H
#define MOVE_H

#include <optional>
#include <string>
#include "Piece.h"

class GameState;

struct Move {
    int fromRow, fromCol, toRow, toCol;
    std::optional<PieceType> promotion;
    bool isCastling;
    bool isEnPassant;

    Move(int fr=0, int fc=0, int tr=0, int tc=0,
         std::optional<PieceType> promo = std::nullopt,
         bool castling = false,
         bool enPassant = false) noexcept;

     std::string toUCI() const;

    static std::optional<Move> fromUCI(const std::string& uci);

    static std::optional<Move> fromUCIInPosition(const std::string& uci,
                                                 const GameState& state);

    bool sameSquaresAndPromo(const Move& o) const noexcept {
        return fromRow == o.fromRow && fromCol == o.fromCol &&
               toRow   == o.toRow   && toCol   == o.toCol &&
               promotion == o.promotion;
    }
};

#endif //MOVE_H
