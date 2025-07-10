#ifndef PIECE_H
#define PIECE_H

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

enum class Color {
    White,
    Black
};

class Piece {
public:
    Piece(PieceType type, Color color) noexcept;

    PieceType type() const noexcept;

    Color color() const noexcept;

    char symbol() const noexcept;

    int value() const noexcept;

private:
    PieceType type_;
    Color color_;
};

#endif //PIECE_H
