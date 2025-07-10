cmake_minimum_required(VERSION 3.31)
project(GameOfChess)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        REQUIRED)

add_executable(GameOfChess main.cpp
        src/Board.cpp
        src/Board.h
        src/Piece.cpp
        src/Piece.h
        src/Move.cpp
        src/Move.h
        src/ChessBoardWidget.cpp
        src/ChessBoardWidget.h
        src/MoveGen.cpp
        src/MoveGen.h
        src/GameState.cpp
        src/GameState.h
        src/MenuWindow.cpp
        src/MenuWindow.h
)
target_link_libraries(GameOfChess
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg
)

