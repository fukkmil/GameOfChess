# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.30)
project(ChessTests)

set(CMAKE_CXX_STANDARD 26)

find_package(GTest REQUIRED)
include(GoogleTest)

set(SRC_FILES
        ../src/Board.cpp
        ../src/Piece.cpp
        ../src/Move.cpp
        ../src/GameState.cpp
        ../src/MoveGen.cpp
)

add_executable(chess_tests
        ${SRC_FILES}
        BoardTest.cpp
        PieceTest.cpp
        MoveTest.cpp
        GameStateTest.cpp
        MoveGenTest.cpp
)

target_include_directories(chess_tests PRIVATE ../src)
target_link_libraries(chess_tests GTest::GTest GTest::Main)

gtest_discover_tests(chess_tests)