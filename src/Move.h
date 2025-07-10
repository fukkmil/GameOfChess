#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <optional>
#include "Piece.h"

class Move {
public:
    int fromRow, fromCol;
    int toRow, toCol;

    std::optional<PieceType> promotion;

    bool isCastling = false;
    bool isEnPassant = false;

    Move(int fr, int fc, int tr, int tc,
         std::optional<PieceType> promo = std::nullopt,
         bool castling = false,
         bool enPassant = false) noexcept;

    std::string toUCI() const;

    static std::optional<Move> fromUCI(const std::string &uci);
};

#endif //MOVE_H
