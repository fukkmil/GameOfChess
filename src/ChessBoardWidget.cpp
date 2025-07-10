#include "ChessBoardWidget.h"
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>
#include "MoveGen.h"

ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent)
      , gameState_()
      , sideToMove_(gameState_.sideToMove()) {
    setBackgroundRole(QPalette::Base);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    loadPixmaps();
    newGame();
}

void ChessBoardWidget::newGame() {
    gameState_ = GameState();
    sideToMove_ = gameState_.sideToMove();
    selectedCell_.reset();
    legalMoves_.clear();
    update();
}

bool ChessBoardWidget::canUndo() const {
    return !gameState_.history().empty();
}

void ChessBoardWidget::undoMove() {
    if (gameState_.undoMove()) {
        sideToMove_ = gameState_.sideToMove();
        selectedCell_.reset();
        legalMoves_.clear();
        update();
    }
}

QSize ChessBoardWidget::minimumSizeHint() const {
    return QSize(320, 320);
}

bool ChessBoardWidget::pixelToCell(const QPoint &pt, int *row, int *col) const {
    int rows = Board::SIZE;
    int cols = Board::SIZE;
    int cellSize = qMin(width() / cols, height() / rows);
    int xOffset = (width() - cellSize * cols) / 2;
    int yOffset = (height() - cellSize * rows) / 2;
    int x = pt.x() - xOffset;
    int y = pt.y() - yOffset;
    // qDebug() << "pixelToCell: pt=" << pt << "-> xOffset,yOffset=" << xOffset << yOffset << " x,y=" << x << y;
    if (x < 0 || y < 0) return false;
    int c = x / cellSize;
    int r = rows - 1 - (y / cellSize);
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    *row = r;
    *col = c;
    // qDebug() << "pixelToCell: cell=" << r << col;
    return true;
}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event) {
    //qDebug() << "Mouse pressed at" << event->pos();
    int row, col;
    if (!pixelToCell(event->pos(), &row, &col)) return;
    const Board &board = gameState_.board();
    //qDebug() << "Clicked cell: row" << row << "col" << col;

    if (!selectedCell_) {
        auto opt = board.pieceAt(row, col);
        if (opt && opt->color() == sideToMove_) {
            selectedCell_ = QPoint(row, col);
            //qDebug() << "Selected cell:" << row << col;
            auto allMoves = MoveGenerator::generateLegal(gameState_);
            legalMoves_.clear();
            for (const auto &m: allMoves) {
                if (m.fromRow == row && m.fromCol == col) {
                    legalMoves_.push_back(m);
                }
            }
            //qDebug() << "Legal moves count:" << legalMoves_.size();
        }
    } else {
        bool moved = false;
        for (const auto &m: legalMoves_) {
            if (m.toRow == row && m.toCol == col) {
                //  qDebug() << "Move applied from" << m.fromRow << m.fromCol << "to" << m.toRow << m.toCol;
                gameState_.applyMove(m);
                sideToMove_ = gameState_.sideToMove();
                moved = true;
                break;
            }
        }
        selectedCell_.reset();
        legalMoves_.clear();

        if (moved) {
            if (gameState_.repetitionCount() >= 3) {
                QMessageBox::information(this, tr("Ничья"), tr("Ничья повторением ходов."));
            }
            auto nextMoves = MoveGenerator::generateLegal(gameState_);
            if (nextMoves.empty()) {
                bool inCheck = MoveGenerator::isInCheck(gameState_.board(), sideToMove_);
                if (inCheck) {
                    Color winner = (sideToMove_ == Color::White ? Color::Black : Color::White);
                    QString msg = tr("Шах и мат! Победили %1")
                                      .arg(winner == Color::White ? tr("Белые") : tr("Чёрные"));
                    QMessageBox::information(this, tr("Мат"), msg);
                } else {
                    QMessageBox::information(this, tr("Ничья"), tr("Пат! Ничья."));
                }
            }
        }
    }
    update();
}

void ChessBoardWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int rows = Board::SIZE;
    int cols = Board::SIZE;
    int cellW = width() / cols;
    int cellH = height() / rows;
    int cellSize = qMin(cellW, cellH);
    int xOffset = (width() - cellSize * cols) / 2;
    int yOffset = (height() - cellSize * rows) / 2;

    QColor light(240, 217, 181);
    QColor dark(181, 136, 99);

    if (selectedCell_) {
        int sr = selectedCell_->x();
        int sc = selectedCell_->y();
        QRect selRect(xOffset + sc * cellSize,
                      yOffset + (rows - 1 - sr) * cellSize,
                      cellSize, cellSize);
        painter.fillRect(selRect, QColor(255, 255, 0, 100));
        for (const auto &m: legalMoves_) {
            QRect mr(xOffset + m.toCol * cellSize,
                     yOffset + (rows - 1 - m.toRow) * cellSize,
                     cellSize, cellSize);
            painter.fillRect(mr, QColor(0, 255, 0, 100));
        }
    }
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QRect cellRect(xOffset + c * cellSize,
                           yOffset + (rows - 1 - r) * cellSize,
                           cellSize, cellSize);
            bool isLight = ((r + c) % 2 == 0);
            painter.fillRect(cellRect, isLight ? light : dark);
            auto opt = gameState_.board().pieceAt(r, c);
            if (opt) {
                Piece piece = *opt;
                int t = static_cast<int>(piece.type());
                int colIdx = (piece.color() == Color::White) ? 0 : 1;
                QRect pixRect = cellRect.marginsRemoved(QMargins(4, 4, 4, 4));
                painter.drawPixmap(pixRect, piecePixmaps_[t][colIdx]);
            }
        }
    }
}

void ChessBoardWidget::loadPixmaps() {
    const QString colors[2] = {"white", "black"};
    const QString types[6] = {"king", "queen", "rook", "bishop", "knight", "pawn"};
    for (int t = 0; t < 6; ++t) {
        for (int c = 0; c < 2; ++c) {
            QString path = QStringLiteral("../images/%1_%2.svg").arg(colors[c]).arg(types[t]);
            QSvgRenderer renderer(path);
            QPixmap pixmap(64, 64);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            renderer.render(&painter);
            piecePixmaps_[t][c] = pixmap;
        }
    }
}
