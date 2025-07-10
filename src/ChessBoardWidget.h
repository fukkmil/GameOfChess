#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <optional>
#include <vector>
#include "GameState.h"

class ChessBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit ChessBoardWidget(QWidget* parent = nullptr);
    ~ChessBoardWidget() override = default;

    void newGame();
    bool canUndo() const;
    void undoMove();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    QSize minimumSizeHint() const override;

private:
    bool pixelToCell(const QPoint& pt, int* row, int* col) const;

    GameState gameState_;
    Color sideToMove_;
    std::optional<QPoint> selectedCell_;
    std::vector<Move> legalMoves_;

    QPixmap piecePixmaps_[6][2];
    void loadPixmaps();
};
#endif //CHESSBOARDWIDGET_H
