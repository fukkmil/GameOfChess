#include "ChessBoardWidget.h"
#include "MoveGen.h"
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QMouseEvent>
#include <QMessageBox>


ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent)
      , gameState_()
      , sideToMove_(Color::White)
      , animating_(false)
      , animProgress_(0.0)
      , animation_(new QPropertyAnimation(this, "animationProgress", this))
      , checkTimer_(new QTimer(this))
      , flashOn_(false)
      , flashCount_(0) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    loadPixmaps();
    newGame();

    animation_->setDuration(150);
    animation_->setEasingCurve(QEasingCurve::InOutQuad);
    connect(animation_, &QPropertyAnimation::finished,
            this, &ChessBoardWidget::onAnimationFinished);

    checkTimer_->setInterval(200);
    connect(checkTimer_, &QTimer::timeout, this, &ChessBoardWidget::onCheckFlash);
}

ChessBoardWidget::~ChessBoardWidget() {
    delete animation_;
}

void ChessBoardWidget::newGame() {
    if (animation_->state() == QAbstractAnimation::Running)
        animation_->stop();
    checkTimer_->stop();
    animating_ = false;
    animProgress_ = 0;
    currentMove_.reset();
    flashOn_ = false;
    flashCount_ = 0;

    gameState_ = GameState();
    sideToMove_ = gameState_.sideToMove();
    selectedCell_.reset();
    legalMoves_.clear();
    update();
}

bool ChessBoardWidget::canUndo() const {
    return !gameState_.history().empty() && !animating_;
}

void ChessBoardWidget::undoMove() {
    if (animating_) return;
    if (gameState_.undoMove()) {
        sideToMove_ = gameState_.sideToMove();
        selectedCell_.reset();
        legalMoves_.clear();
        update();
    }
}

QSize ChessBoardWidget::minimumSizeHint() const {
    return {320, 320};
}

bool ChessBoardWidget::pixelToCell(const QPoint &pt, int *row, int *col) const {
    int rows = Board::SIZE;
    int cols = Board::SIZE;
    int cellSize = qMin(width() / cols, height() / rows);
    int xOffset = (width() - cellSize * cols) / 2;
    int yOffset = (height() - cellSize * rows) / 2;
    int x = pt.x() - xOffset;
    int y = pt.y() - yOffset;
    if (x < 0 || y < 0) return false;
    int c = x / cellSize;
    int r = rows - 1 - (y / cellSize);
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    *row = r;
    *col = c;
    return true;
}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event) {
    if (animating_) return;
    int row, col;
    if (!pixelToCell(event->pos(), &row, &col)) return;
    const Board &board = gameState_.board();

    if (!selectedCell_) {
        auto opt = board.pieceAt(row, col);
        if (opt && opt->color() == sideToMove_) {
            selectedCell_ = QPoint(row, col);
            legalMoves_.clear();
            auto all = MoveGenerator::generateLegal(gameState_);
            for (const auto &m: all) {
                if (m.fromRow == row && m.fromCol == col)
                    legalMoves_.push_back(m);
            }
        }
    } else {
        for (const auto &m: legalMoves_) {
            if (m.toRow == row && m.toCol == col) {
                animateMove(m);
                break;
            }
        }
        selectedCell_.reset();
        legalMoves_.clear();
    }
    update();
}

void ChessBoardWidget::animateMove(const Move &move) {
    animating_ = true;
    currentMove_ = move;

    int rows = Board::SIZE;
    int cols = Board::SIZE;
    int cellSize = qMin(width() / cols, height() / rows);
    int xOffset = (width() - cellSize * cols) / 2;
    int yOffset = (height() - cellSize * rows) / 2;
    QPoint fromPt(xOffset + move.fromCol * cellSize + cellSize / 2,
                  yOffset + (rows - 1 - move.fromRow) * cellSize + cellSize / 2);
    QPoint toPt(xOffset + move.toCol * cellSize + cellSize / 2,
                yOffset + (rows - 1 - move.toRow) * cellSize + cellSize / 2);
    startPos_ = fromPt;
    endPos_ = toPt;
    animation_->stop();
    animation_->setStartValue(0.0);
    animation_->setEndValue(1.0);
    animation_->start();
}

qreal ChessBoardWidget::animationProgress() const {
    return animProgress_;
}

void ChessBoardWidget::setAnimationProgress(qreal p) {
    animProgress_ = p;
    update();
}

void ChessBoardWidget::onAnimationFinished() {
    if (!currentMove_) return;
    gameState_.applyMove(*currentMove_);
    sideToMove_ = gameState_.sideToMove();
    animating_ = false;
    animProgress_ = 0;
    currentMove_.reset();
    update();

    auto nextMoves = MoveGenerator::generateLegal(gameState_);
    if (nextMoves.empty()) {
        bool inCheck = MoveGenerator::isInCheck(gameState_.board(), sideToMove_);
        if (inCheck) {
            flashOn_ = false;
            flashCount_ = 0;
            checkTimer_->start();
            Color winner = (sideToMove_ == Color::White ? Color::Black : Color::White);
            QMessageBox::information(this, tr("Мат"),
                                     tr("Шах-мат! Победили %1").arg(
                                         winner == Color::White ? tr("Белые") : tr("Чёрные")));
        } else if (gameState_.repetitionCount() >= 3) {
            QMessageBox::information(this, tr("Ничья"), tr("Ничья по трёхкратному повторению."));
        } else {
            QMessageBox::information(this, tr("Ничья"), tr("Пат! Ничья."));
        }
    } else {
        bool inCheck = MoveGenerator::isInCheck(gameState_.board(), sideToMove_);
        if (inCheck) {
            flashOn_ = false;
            flashCount_ = 0;
            checkTimer_->start();
        }
    }
}

void ChessBoardWidget::onCheckFlash() {
    flashOn_ = !flashOn_;
    flashCount_++;
    if (flashCount_ >= 6) {
        checkTimer_->stop();
        flashOn_ = false;
    }
    update();
}

void ChessBoardWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int rows = Board::SIZE;
    int cols = Board::SIZE;
    int cellSize = qMin(width() / cols, height() / rows);
    int xOffset = (width() - cellSize * cols) / 2;
    int yOffset = (height() - cellSize * rows) / 2;
    QColor light(240, 217, 181), dark(181, 136, 99);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QRect cell(xOffset + c * cellSize,
                       yOffset + (rows - 1 - r) * cellSize,
                       cellSize, cellSize);
            painter.fillRect(cell, ((r + c) % 2 == 0) ? light : dark);
        }
    }

    if (flashOn_) {
        int kr = -1, kc = -1;
        for (int r = 0; r < Board::SIZE; ++r) {
            for (int c = 0; c < Board::SIZE; ++c) {
                auto p = gameState_.board().pieceAt(r, c);
                if (p && p->type() == PieceType::King && p->color() == sideToMove_) {
                    kr = r;
                    kc = c;
                }
            }
        }
        if (kr >= 0) {
            int cellSize = qMin(width() / Board::SIZE, height() / Board::SIZE);
            int xOffset = (width() - cellSize * Board::SIZE) / 2;
            int yOffset = (height() - cellSize * Board::SIZE) / 2;
            QRect kingCell(
                xOffset + kc * cellSize,
                yOffset + (Board::SIZE - 1 - kr) * cellSize,
                cellSize, cellSize
            );
            painter.fillRect(kingCell, QColor(255, 0, 0, 100));
        }
    }

    if (selectedCell_) {
        int sr = selectedCell_->x();
        int sc = selectedCell_->y();
        QRect sel(xOffset + sc * cellSize,
                  yOffset + (rows - 1 - sr) * cellSize,
                  cellSize, cellSize);
        painter.fillRect(sel, QColor(255, 255, 0, 100));
    }

    if (!animating_) {
        for (const auto &m: legalMoves_) {
            int tr = m.toRow, tc = m.toCol;
            QRect cellRect(
                xOffset + tc * cellSize,
                yOffset + (rows - 1 - tr) * cellSize,
                cellSize,
                cellSize
            );
            if (gameState_.board().pieceAt(tr, tc).has_value() || m.isEnPassant) {
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen(QColor(128, 128, 128, 180), 4));
                QRect inner = cellRect.marginsRemoved(QMargins(4, 4, 4, 4));
                painter.drawEllipse(inner);
            } else {
                painter.setBrush(QBrush(QColor(128, 128, 128, 180)));
                painter.setPen(Qt::NoPen);
                QPoint center(
                    cellRect.x() + cellSize / 2,
                    cellRect.y() + cellSize / 2
                );
                int radius = cellSize / 8;
                painter.drawEllipse(center, radius, radius);
            }
        }
    }

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            auto opt = gameState_.board().pieceAt(r, c);
            if (!opt) continue;
            if (animating_ && currentMove_) {
                if (r == currentMove_->fromRow && c == currentMove_->fromCol) continue;
                if (r == currentMove_->toRow && c == currentMove_->toCol) continue;
            }
            Piece p = *opt;
            int t = static_cast<int>(p.type());
            int idx = (p.color() == Color::White) ? 0 : 1;
            QRect img(xOffset + c * cellSize + 4,
                      yOffset + (rows - 1 - r) * cellSize + 4,
                      cellSize - 8, cellSize - 8);
            painter.drawPixmap(img, piecePixmaps_[t][idx]);
        }
    }

    if (animating_ && currentMove_) {
        Move m = *currentMove_;
        Piece moving = *gameState_.board().pieceAt(m.fromRow, m.fromCol);
        int t = static_cast<int>(moving.type());
        int idx = (moving.color() == Color::White) ? 0 : 1;
        QPointF pos = startPos_ + (endPos_ - startPos_) * animProgress_;
        QRect img(QPoint(static_cast<int>(pos.x() - cellSize / 2 + 4), static_cast<int>(pos.y() - cellSize / 2 + 4)),
                  QSize(cellSize - 8, cellSize - 8));
        painter.drawPixmap(img, piecePixmaps_[t][idx]);
    }
}

void ChessBoardWidget::loadPixmaps() {
    const QString colors[2] = {"white", "black"};
    const QString types[6] = {"king", "queen", "rook", "bishop", "knight", "pawn"};
    for (int t = 0; t < 6; ++t) {
        for (int c = 0; c < 2; ++c) {
            QString path = QStringLiteral("../images/%1_%2.svg").arg(colors[c], types[t]);
            QSvgRenderer renderer(path);
            QPixmap pixmap(64, 64);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            renderer.render(&painter);
            piecePixmaps_[t][c] = pixmap;
        }
    }
}
