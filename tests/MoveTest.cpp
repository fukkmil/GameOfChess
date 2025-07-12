#include <gtest/gtest.h>
#include "../src/Move.h"

TEST(MoveTest, UCIConversion) {
    Move m(1, 2, 3, 4, PieceType::Queen);
    EXPECT_EQ(m.toUCI(), "b2d4q");

    Move castle(0, 4, 0, 6, std::nullopt, true);
    EXPECT_EQ(castle.toUCI(), "e1g1");
}

TEST(MoveTest, UCIParsing) {
    auto optMove = Move::fromUCI("g1f3");
    ASSERT_TRUE(optMove.has_value());
    EXPECT_EQ(optMove->fromRow, 0);
    EXPECT_EQ(optMove->fromCol, 6);
    EXPECT_EQ(optMove->toRow, 2);
    EXPECT_EQ(optMove->toCol, 5);

    auto invalid = Move::fromUCI("a9b2");
    EXPECT_FALSE(invalid.has_value());
}