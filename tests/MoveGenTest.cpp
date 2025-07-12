#include <gtest/gtest.h>
#include "../src/MoveGen.h"
#include "../src/GameState.h"

TEST(MoveGenTest, PawnMoves) {
    GameState state;
    auto moves = MoveGenerator::generateLegal(state);

    // У белых должно быть 20 начальных ходов
    EXPECT_EQ(moves.size(), 20);

    // Проверка хода пешки e2-e4
    bool found = false;
    for (const auto& move : moves) {
        if (move.fromRow == 1 && move.fromCol == 4 && move.toRow == 3 && move.toCol == 4) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST(MoveGenTest, CheckDetection) {
    GameState state;

    // Создаем позицию с шахом (ферзь атакует короля)
    state.applyMove(Move(1, 4, 3, 4));  // e4
    state.applyMove(Move(6, 3, 4, 3));  // d5
    state.applyMove(Move(0, 3, 4, 7));  // Фd1-h5 (атака на черного короля)

    // Проверяем шах через isInCheck()
    EXPECT_TRUE(MoveGenerator::isInCheck(state.board(), Color::Black));

    // Альтернативно: проверяем что у черных нет легальных ходов (мат)
    auto blackMoves = MoveGenerator::generateLegal(state);
    EXPECT_TRUE(blackMoves.empty()); // Это уже будет проверка на мат
}

TEST(MoveGenTest, CastlingGeneration) {
    GameState state;

    // Освобождаем путь для рокировки
    state.applyMove(Move(1, 6, 3, 6));  // g3
    state.applyMove(Move(0, 5, 2, 7));  // Сf1-h3
    state.applyMove(Move(0, 1, 2, 2));  // Кb1-c3

    auto moves = MoveGenerator::generateLegal(state);
    bool castlingFound = false;
    for (const auto& move : moves) {
        if (move.isCastling) {
            castlingFound = true;
            break;
        }
    }
    EXPECT_TRUE(castlingFound);
}