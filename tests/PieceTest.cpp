#include <gtest/gtest.h>
#include "../src/Piece.h"

TEST(PieceTest, SymbolRepresentation) {
    Piece whiteQueen(PieceType::Queen, Color::White);
    Piece blackKnight(PieceType::Knight, Color::Black);

    EXPECT_EQ(whiteQueen.symbol(), 'Q');
    EXPECT_EQ(blackKnight.symbol(), 'n');
}

TEST(PieceTest, PieceValues) {
    Piece rook(PieceType::Rook, Color::White);
    Piece bishop(PieceType::Bishop, Color::Black);
    Piece pawn(PieceType::Pawn, Color::White);

    EXPECT_EQ(rook.value(), 5);
    EXPECT_EQ(bishop.value(), 3);
    EXPECT_EQ(pawn.value(), 1);
}