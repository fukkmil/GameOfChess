#include "GameState.h"
#include <sstream>

GameState::GameState()
    : board_(), sideToMove_(Color::White),
      whiteKingSideCastle_(true), whiteQueenSideCastle_(true),
      blackKingSideCastle_(true), blackQueenSideCastle_(true),
      enPassantTarget_(std::nullopt), halfmoveClock_(0) {
    std::string key = positionKey();
    repetitionCounts_[key] = 1;
}

const Board &GameState::board() const noexcept { return board_; }
Color GameState::sideToMove() const noexcept { return sideToMove_; }

bool GameState::canCastleKingSide(Color color) const noexcept {
    return (color == Color::White) ? whiteKingSideCastle_ : blackKingSideCastle_;
}

bool GameState::canCastleQueenSide(Color color) const noexcept {
    return (color == Color::White) ? whiteQueenSideCastle_ : blackQueenSideCastle_;
}

std::optional<std::pair<int, int> > GameState::enPassantTarget() const noexcept {
    return enPassantTarget_;
}

int GameState::halfmoveClock() const noexcept { return halfmoveClock_; }
const std::vector<Move> &GameState::history() const noexcept { return history_; }

int GameState::repetitionCount() const noexcept {
    auto key = positionKey();
    auto it = repetitionCounts_.find(key);
    return (it != repetitionCounts_.end() ? it->second : 0);
}

std::string GameState::positionKey() const {
    std::ostringstream oss;
    for (int r = Board::SIZE - 1; r >= 0; --r) {
        int empty = 0;
        for (int c = 0; c < Board::SIZE; ++c) {
            auto opt = board_.pieceAt(r, c);
            if (opt) {
                if (empty) {
                    oss << empty;
                    empty = 0;
                }
                oss << opt->symbol();
            } else {
                ++empty;
            }
        }
        if (empty) oss << empty;
        if (r > 0) oss << '/';
    }
    oss << ' ' << (sideToMove_ == Color::White ? 'w' : 'b');
    oss << ' ';
    std::string cr;
    if (whiteKingSideCastle_) cr += 'K';
    if (whiteQueenSideCastle_) cr += 'Q';
    if (blackKingSideCastle_) cr += 'k';
    if (blackQueenSideCastle_) cr += 'q';
    oss << (cr.empty() ? "-" : cr);
    oss << ' ';
    if (enPassantTarget_) {
        char file = 'a' + enPassantTarget_->second;
        char rank = '1' + enPassantTarget_->first;
        oss << file << rank;
    } else {
        oss << '-';
    }
    return oss.str();
}

void GameState::applyMove(const Move &move) {
    StateSnapshot snap{
        board_, sideToMove_,
        whiteKingSideCastle_, whiteQueenSideCastle_,
        blackKingSideCastle_, blackQueenSideCastle_,
        enPassantTarget_, halfmoveClock_,
        history_, repetitionCounts_
    };
    snapshots_.push_back(std::move(snap));

    auto movingOpt = board_.pieceAt(move.fromRow, move.fromCol);
    if (!movingOpt) return;
    PieceType pt = movingOpt->type();
    Color movingColor = movingOpt->color();
    std::optional<Piece> captured;
    if (move.isEnPassant) captured = board_.pieceAt(move.fromRow, move.toCol);
    else captured = board_.pieceAt(move.toRow, move.toCol);
    bool isCapture = captured.has_value();

    board_.applyMove(move);

    if (pt == PieceType::King) {
        if (movingColor == Color::White)
            whiteKingSideCastle_ = whiteQueenSideCastle_ = false;
        else
            blackKingSideCastle_ = blackQueenSideCastle_ = false;
    } else if (pt == PieceType::Rook) {
        if (movingColor == Color::White) {
            if (move.fromRow == 0 && move.fromCol == 7) whiteKingSideCastle_ = false;
            if (move.fromRow == 0 && move.fromCol == 0) whiteQueenSideCastle_ = false;
        } else {
            if (move.fromRow == 7 && move.fromCol == 7) blackKingSideCastle_ = false;
            if (move.fromRow == 7 && move.fromCol == 0) blackQueenSideCastle_ = false;
        }
    }
    if (pt == PieceType::Pawn && std::abs(move.toRow - move.fromRow) == 2) {
        enPassantTarget_ = std::make_pair((move.fromRow + move.toRow) / 2, move.fromCol);
    } else {
        enPassantTarget_.reset();
    }
    if (isCapture && captured->type() == PieceType::Rook) {
        auto [cr, cc] = move.isEnPassant
                            ? std::make_pair(move.fromRow, move.toCol)
                            : std::make_pair(move.toRow, move.toCol);
        if (captured->color() == Color::White) {
            if (cr == 0 && cc == 7) whiteKingSideCastle_ = false;
            if (cr == 0 && cc == 0) whiteQueenSideCastle_ = false;
        } else {
            if (cr == 7 && cc == 7) blackKingSideCastle_ = false;
            if (cr == 7 && cc == 0) blackQueenSideCastle_ = false;
        }
    }
    if (pt == PieceType::Pawn || isCapture) halfmoveClock_ = 0;
    else ++halfmoveClock_;

    history_.push_back(move);
    sideToMove_ = (sideToMove_ == Color::White ? Color::Black : Color::White);

    std::string key = positionKey();
    repetitionCounts_[key]++;
}

bool GameState::undoMove() {
    if (snapshots_.empty()) return false;
    StateSnapshot snap = std::move(snapshots_.back());
    snapshots_.pop_back();

    board_ = std::move(snap.board);
    sideToMove_ = snap.side;
    whiteKingSideCastle_ = snap.wKS;
    whiteQueenSideCastle_ = snap.wQS;
    blackKingSideCastle_ = snap.bKS;
    blackQueenSideCastle_ = snap.bQS;
    enPassantTarget_ = snap.enPassant;
    halfmoveClock_ = snap.halfmoveClock;
    history_ = std::move(snap.history);
    repetitionCounts_ = std::move(snap.repetitionCounts);
    return true;
}
