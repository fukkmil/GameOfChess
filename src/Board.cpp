#include "Board.h"

const std::optional<Piece> &Board::pieceAt(int row, int col) const {
    return board_[row][col];
}

std::optional<Piece> &Board::pieceAt(int row, int col) {
    return board_[row][col];
}

bool Board::isInside(int row, int col) const noexcept {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

void Board::initialize() {
    for (auto &row: board_) {
        for (auto &cell: row) {
            cell.reset();
        }
    }

    board_[0][0] = Piece(PieceType::Rook, Color::White);
    board_[0][1] = Piece(PieceType::Knight, Color::White);
    board_[0][2] = Piece(PieceType::Bishop, Color::White);
    board_[0][3] = Piece(PieceType::Queen, Color::White);
    board_[0][4] = Piece(PieceType::King, Color::White);
    board_[0][5] = Piece(PieceType::Bishop, Color::White);
    board_[0][6] = Piece(PieceType::Knight, Color::White);
    board_[0][7] = Piece(PieceType::Rook, Color::White);
    for (int col = 0; col < SIZE; ++col) {
        board_[1][col] = Piece(PieceType::Pawn, Color::White);
    }

    board_[7][0] = Piece(PieceType::Rook, Color::Black);
    board_[7][1] = Piece(PieceType::Knight, Color::Black);
    board_[7][2] = Piece(PieceType::Bishop, Color::Black);
    board_[7][3] = Piece(PieceType::Queen, Color::Black);
    board_[7][4] = Piece(PieceType::King, Color::Black);
    board_[7][5] = Piece(PieceType::Bishop, Color::Black);
    board_[7][6] = Piece(PieceType::Knight, Color::Black);
    board_[7][7] = Piece(PieceType::Rook, Color::Black);
    for (int col = 0; col < SIZE; ++col) {
        board_[6][col] = Piece(PieceType::Pawn, Color::Black);
    }
}

void Board::applyMove(const Move& move) {
    if (!isInside(move.fromRow, move.fromCol) || !isInside(move.toRow, move.toCol)) {
        return;
    }
    auto movingOpt = board_[move.fromRow][move.fromCol];
    if (!movingOpt) return;
    Piece moving = *movingOpt;

    if (move.isCastling && moving.type() == PieceType::King) {
        int row = move.fromRow;
        if (move.toCol == 6) {
            board_[row][5] = board_[row][7];
            board_[row][7].reset();
        } else if (move.toCol == 2) {
            board_[row][3] = board_[row][0];
            board_[row][0].reset();
        }
    }

    if (move.isEnPassant && moving.type() == PieceType::Pawn) {
        board_[move.fromRow][move.toCol].reset();
    }

    if (moving.type() == PieceType::Pawn && move.promotion) {
        board_[move.toRow][move.toCol] = Piece(*move.promotion, moving.color());
    } else {
        board_[move.toRow][move.toCol] = movingOpt;
    }

    board_[move.fromRow][move.fromCol].reset();
}
