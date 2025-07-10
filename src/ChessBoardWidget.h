#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <optional>
#include <vector>
#include <QPropertyAnimation>
#include <QTimer>
#include <QString>
#include "GameState.h"

class ChessBoardWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal animationProgress READ animationProgress WRITE setAnimationProgress)

public:
    explicit ChessBoardWidget(QWidget *parent = nullptr);

    ~ChessBoardWidget() override;

    void newGame();

    [[nodiscard]] bool canUndo() const;

    void undoMove();

    [[nodiscard]] Color sideToMove() const noexcept;

signals:
    void moveMade(const QString &san);
    void gameReset();

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    [[nodiscard]] QSize minimumSizeHint() const override;

private slots:
    void onAnimationFinished();

    void onCheckFlash();

private:
    bool pixelToCell(const QPoint &pt, int *row, int *col) const;

    [[nodiscard]] qreal animationProgress() const;

    void setAnimationProgress(qreal p);

    void animateMove(const Move &move);

    GameState gameState_;
    bool gameOver_;
    Color sideToMove_;
    std::optional<QPoint> selectedCell_;
    std::vector<Move> legalMoves_;

    bool animating_;
    std::optional<Move> currentMove_;
    QPointF startPos_, endPos_;
    qreal animProgress_;
    QPropertyAnimation *animation_;

    QTimer *checkTimer_;
    bool flashOn_;
    int flashCount_;

    QPixmap piecePixmaps_[6][2];

    void loadPixmaps();

    static QString moveToSan(const GameState &state, const Move &move);
};

#endif //CHESSBOARDWIDGET_H
