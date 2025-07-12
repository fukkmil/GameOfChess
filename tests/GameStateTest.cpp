#include <gtest/gtest.h>
#include "../src/GameState.h"
#include "../src/MoveGen.h"

TEST(GameStateTest, CastlingRights) {
    GameState state;
    EXPECT_TRUE(state.canCastleKingSide(Color::White));

    // ход ладьой - должна потерять рокировку
    Move rookMove(0, 0, 1, 0);
    state.applyMove(rookMove);
    EXPECT_FALSE(state.canCastleQueenSide(Color::White));
}

TEST(GameStateTest, RepetitionDetection) {
    GameState state;
    Move move1(1, 4, 2, 4);  // e2-e3
    Move move2(6, 4, 5, 4);  // e7-e6

    state.applyMove(move1);
    state.applyMove(move2);
    state.undoMove();
    state.undoMove();

    EXPECT_EQ(state.repetitionCount(), 2);
}

TEST(GameStateTest, EnPassantState) {
    GameState state;
    Move doubleMove(1, 0, 3, 0);  // a2-a4
    state.applyMove(doubleMove);

    auto epTarget = state.enPassantTarget();
    ASSERT_TRUE(epTarget.has_value());
    EXPECT_EQ(epTarget->first, 2);
    EXPECT_EQ(epTarget->second, 0);
}