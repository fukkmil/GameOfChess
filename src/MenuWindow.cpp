#include "MenuWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
      , board_(new ChessBoardWidget(this))
      , historyList_(new QListWidget(this))
      , resignButton_(new QPushButton(tr("Сдаться"), this))
      , halfmoveCount_(0) {
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(board_, 3);

    auto *sideLayout = new QVBoxLayout();
    sideLayout->addWidget(historyList_);
    sideLayout->addWidget(resignButton_);
    mainLayout->addLayout(sideLayout, 1);

    connect(resignButton_, &QPushButton::clicked, this, &MenuWindow::onResign);
    connect(board_, &ChessBoardWidget::moveMade, this, &MenuWindow::onMoveMade);
    connect(board_, &ChessBoardWidget::gameReset, this, [this]() {
        historyList_->clear();
        halfmoveCount_ = 0;
    });
}

void MenuWindow::onResign() {
    Color loser = board_->sideToMove();
    QString winner = (loser == Color::White) ? tr("Чёрные") : tr("Белые");
    QMessageBox::information(this, tr("Поражение"),
                             tr("%1 победили").arg(winner));
    board_->newGame();
    historyList_->clear();
    halfmoveCount_ = 0;
}

void MenuWindow::onMoveMade(const QString &san) {
    if (halfmoveCount_ % 2 == 0) {
        QString text = QString::number(halfmoveCount_ / 2 + 1) + ". " + san;
        historyList_->addItem(text);
    } else {
        auto *item = historyList_->item(halfmoveCount_ / 2);
        if (item) item->setText(item->text() + "  " + san);
    }
    halfmoveCount_++;
}

void MenuWindow::onReturnToMenu() {
    auto* mainMenu = new MainMenuWidget();
    mainMenu->setAttribute(Qt::WA_DeleteOnClose);
    mainMenu->show();
    close();
}