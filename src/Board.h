#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <optional>
#include "Piece.h"
#include "Move.h"

class Board {
public:
    static constexpr int SIZE = 8;

    Board() { initialize(); }

    Board(const Board &) = default;

    Board &operator=(const Board &) = default;

    ~Board() = default;

    const std::optional<Piece> &pieceAt(int row, int col) const;

    std::optional<Piece> &pieceAt(int row, int col);

    void initialize();

    void applyMove(const Move &move);

    bool isInside(int row, int col) const noexcept;

private:
    std::array<std::array<std::optional<Piece>, SIZE>, SIZE> board_;
};

#endif // BOARD_H
