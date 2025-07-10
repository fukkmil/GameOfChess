#include "Piece.h"

#include <locale>

Piece::Piece(PieceType type, Color color) noexcept
    : type_(type), color_(color) {
}

PieceType Piece::type() const noexcept {
    return type_;
}

Color Piece::color() const noexcept {
    return color_;
}

char Piece::symbol() const noexcept {
    char c;
    switch (type_) {
        case PieceType::King: c = 'K';
            break;
        case PieceType::Queen: c = 'Q';
            break;
        case PieceType::Rook: c = 'R';
            break;
        case PieceType::Bishop: c = 'B';
            break;
        case PieceType::Knight: c = 'N';
            break;
        case PieceType::Pawn: c = 'P';
            break;
    }
    return (color_ == Color::White)
               ? c
               : static_cast<char>(std::tolower(c));
}

int Piece::value() const noexcept {
    switch (type_) {
        case PieceType::King: return 0;
        case PieceType::Queen: return 9;
        case PieceType::Rook: return 5;
        case PieceType::Bishop: return 3;
        case PieceType::Knight: return 3;
        case PieceType::Pawn: return 1;
    }
    return 0;
}
