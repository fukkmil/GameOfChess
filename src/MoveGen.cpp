#include "MoveGen.h"
#include <algorithm>

static const int ROOK_DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
static const int BISHOP_DIRS[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
static const int KNIGHT_OFFSETS[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

inline Color opposite(Color c) {
    return c == Color::White ? Color::Black : Color::White;
}

std::vector<Move> MoveGenerator::generateLegal(const GameState &state) {
    auto pseudo = generatePseudoLegal(state);
    std::vector<Move> legal;
    for (auto &m: pseudo) {
        GameState copy = state;
        copy.applyMove(m);
        if (!isInCheck(copy.board(), state.sideToMove())) {
            legal.push_back(m);
        }
    }
    return legal;
}

std::vector<Move> MoveGenerator::generatePseudoLegal(const GameState &state) {
    const Board &board = state.board();
    Color side = state.sideToMove();
    std::vector<Move> moves;

    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            auto opt = board.pieceAt(r, c);
            if (!opt || opt->color() != side) continue;
            PieceType pt = opt->type();
            switch (pt) {
                case PieceType::Pawn: {
                    int dir = (side == Color::White ? 1 : -1);
                    int startRow = (side == Color::White ? 1 : 6);
                    int nr = r + dir;
                    if (board.isInside(nr, c) && !board.pieceAt(nr, c)) {
                        if (nr == 0 || nr == 7) {
                            moves.emplace_back(r, c, nr, c, PieceType::Queen);
                            moves.emplace_back(r, c, nr, c, PieceType::Rook);
                            moves.emplace_back(r, c, nr, c, PieceType::Bishop);
                            moves.emplace_back(r, c, nr, c, PieceType::Knight);
                        } else {
                            moves.emplace_back(r, c, nr, c);
                            if (r == startRow) {
                                int nr2 = r + 2 * dir;
                                if (board.isInside(nr2, c) && !board.pieceAt(nr2, c)) {
                                    moves.emplace_back(r, c, nr2, c);
                                }
                            }
                        }
                    }
                    for (int dc: {-1, 1}) {
                        int cc = c + dc;
                        if (!board.isInside(nr, cc)) continue;
                        auto target = board.pieceAt(nr, cc);
                        if (target && target->color() != side) {
                            if (nr == 0 || nr == 7) {
                                moves.emplace_back(r, c, nr, cc, PieceType::Queen);
                                moves.emplace_back(r, c, nr, cc, PieceType::Rook);
                                moves.emplace_back(r, c, nr, cc, PieceType::Bishop);
                                moves.emplace_back(r, c, nr, cc, PieceType::Knight);
                            } else {
                                moves.emplace_back(r, c, nr, cc);
                            }
                        }
                        auto ep = state.enPassantTarget();
                        if (!target && ep && ep->first == nr && ep->second == cc) {
                            Move m(r, c, nr, cc);
                            m.isEnPassant = true;
                            moves.push_back(m);
                        }
                    }
                    break;
                }
                case PieceType::Knight: {
                    for (auto &off: KNIGHT_OFFSETS) {
                        int nr = r + off[0], nc = c + off[1];
                        if (!board.isInside(nr, nc)) continue;
                        auto t = board.pieceAt(nr, nc);
                        if (!t || t->color() != side) moves.emplace_back(r, c, nr, nc);
                    }
                    break;
                }
                case PieceType::Bishop:
                case PieceType::Rook:
                case PieceType::Queen: {
                    auto processDir = [&](int dr, int dc) {
                        for (int step = 1; step < Board::SIZE; ++step) {
                            int nr = r + dr * step, nc = c + dc * step;
                            if (!board.isInside(nr, nc)) break;
                            auto t = board.pieceAt(nr, nc);
                            if (!t) moves.emplace_back(r, c, nr, nc);
                            else {
                                if (t->color() != side) moves.emplace_back(r, c, nr, nc);
                                break;
                            }
                        }
                    };
                    if (pt == PieceType::Bishop || pt == PieceType::Queen)
                        for (auto &d: BISHOP_DIRS) processDir(d[0], d[1]);
                    if (pt == PieceType::Rook || pt == PieceType::Queen)
                        for (auto &d: ROOK_DIRS) processDir(d[0], d[1]);
                    break;
                }
                case PieceType::King: {
                    for (int dr = -1; dr <= 1; ++dr)
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue;
                            int nr = r + dr, nc = c + dc;
                            if (!board.isInside(nr, nc)) continue;
                            auto t = board.pieceAt(nr, nc);
                            if (!t || t->color() != side) moves.emplace_back(r, c, nr, nc);
                        }
                    if (!isInCheck(board, side)) {
                        int backRank = (side == Color::White ? 0 : 7);
                        if (state.canCastleKingSide(side)
                            && !board.pieceAt(backRank, 5)
                            && !board.pieceAt(backRank, 6)
                            && !isAttacked(board, backRank, 5, opposite(side))
                            && !isAttacked(board, backRank, 6, opposite(side))) {
                            Move m(backRank, 4, backRank, 6);
                            m.isCastling = true;
                            moves.push_back(m);
                        }
                        if (state.canCastleQueenSide(side)
                            && !board.pieceAt(backRank, 3)
                            && !board.pieceAt(backRank, 2)
                            && !board.pieceAt(backRank, 1)
                            && !isAttacked(board, backRank, 3, opposite(side))
                            && !isAttacked(board, backRank, 2, opposite(side))) {
                            Move m(backRank, 4, backRank, 2);
                            m.isCastling = true;
                            moves.push_back(m);
                        }
                    }
                    break;
                }
            }
        }
    }
    return moves;
}

bool MoveGenerator::isInCheck(const Board &board, Color color) {
    int kr = -1, kc = -1;
    for (int r = 0; r < Board::SIZE; ++r)
        for (int c = 0; c < Board::SIZE; ++c) {
            auto p = board.pieceAt(r, c);
            if (p && p->type() == PieceType::King && p->color() == color) {
                kr = r;
                kc = c;
            }
        }
    if (kr < 0) return false;
    return isAttacked(board, kr, kc, (color == Color::White ? Color::Black : Color::White));
}

bool MoveGenerator::isAttacked(const Board &board, int row, int col, Color attacker) {
    int dir = (attacker == Color::White ? 1 : -1);
    for (int dc: {-1, 1}) {
        int rr = row - dir, cc = col + dc;
        if (board.isInside(rr, cc)) {
            auto p = board.pieceAt(rr, cc);
            if (p && p->type() == PieceType::Pawn && p->color() == attacker) return true;
        }
    }
    for (auto &off: KNIGHT_OFFSETS) {
        int rr = row + off[0], cc = col + off[1];
        if (board.isInside(rr, cc)) {
            auto p = board.pieceAt(rr, cc);
            if (p && p->type() == PieceType::Knight && p->color() == attacker) return true;
        }
    }
    for (auto &d: ROOK_DIRS) {
        for (int step = 1; step < Board::SIZE; ++step) {
            int rr = row + d[0] * step, cc = col + d[1] * step;
            if (!board.isInside(rr, cc)) break;
            auto p = board.pieceAt(rr, cc);
            if (p) {
                if (p->color() == attacker && (p->type() == PieceType::Rook || p->type() == PieceType::Queen))
                    return
                            true;
                break;
            }
        }
    }
    for (auto &d: BISHOP_DIRS) {
        for (int step = 1; step < Board::SIZE; ++step) {
            int rr = row + d[0] * step, cc = col + d[1] * step;
            if (!board.isInside(rr, cc)) break;
            auto p = board.pieceAt(rr, cc);
            if (p) {
                if (p->color() == attacker && (p->type() == PieceType::Bishop || p->type() == PieceType::Queen))
                    return
                            true;
                break;
            }
        }
    }
    for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int rr = row + dr, cc = col + dc;
            if (!board.isInside(rr, cc)) continue;
            auto p = board.pieceAt(rr, cc);
            if (p && p->type() == PieceType::King && p->color() == attacker) return true;
        }
    return false;
}


