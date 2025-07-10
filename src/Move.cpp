#include "Move.h"
#include <cctype>

Move::Move(int fr, int fc, int tr, int tc,
           std::optional<PieceType> promo,
           bool castling,
           bool enPassant) noexcept
    : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc),
      promotion(promo), isCastling(castling), isEnPassant(enPassant) {
}

namespace {
    inline char fileChar(int col) noexcept { return static_cast<char>('a' + col); }
    inline char rankChar(int row) noexcept { return static_cast<char>('1' + row); }
}

std::string Move::toUCI() const {
    std::string s;
    s += fileChar(fromCol);
    s += rankChar(fromRow);
    s += fileChar(toCol);
    s += rankChar(toRow);
    if (promotion) {
        char p;
        switch (*promotion) {
            case PieceType::Queen: p = 'q';
                break;
            case PieceType::Rook: p = 'r';
                break;
            case PieceType::Bishop: p = 'b';
                break;
            case PieceType::Knight: p = 'n';
                break;
            default: p = '\0';
        }
        if (p) s += p;
    }
    return s;
}

std::optional<Move> Move::fromUCI(const std::string &uci) {
    if (uci.size() < 4) return std::nullopt;
    int fc = uci[0] - 'a';
    int fr = uci[1] - '1';
    int tc = uci[2] - 'a';
    int tr = uci[3] - '1';
    if (fc < 0 || fc > 7 || tc < 0 || tc > 7 || fr < 0 || fr > 7 || tr < 0 || tr > 7) return std::nullopt;

    std::optional<PieceType> promo = std::nullopt;
    if (uci.size() == 5) {
        char c = std::tolower(uci[4]);
        switch (c) {
            case 'q': promo = PieceType::Queen;
                break;
            case 'r': promo = PieceType::Rook;
                break;
            case 'b': promo = PieceType::Bishop;
                break;
            case 'n': promo = PieceType::Knight;
                break;
            default: return std::nullopt;
        }
    }
    return Move(fr, fc, tr, tc, promo);
}
