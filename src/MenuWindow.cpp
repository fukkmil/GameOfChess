#include "MenuWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "MainMenuWidget.h"

MenuWindow::MenuWindow(QWidget* parent)
    : MenuWindow(false, 1600, false, parent) {}

MenuWindow::MenuWindow(bool vsEngine, int engineElo, bool engineIsWhite, QWidget* parent)
    : QWidget(parent)
    , board_(new ChessBoardWidget(this))
    , historyList_(new QListWidget(this))
    , resignButton_(new QPushButton(tr("Сдаться"), this))
    , returnToMenuButton_(new QPushButton(tr("Меню"), this))
    , halfmoveCount_(0)
    , vsEngine_(vsEngine)
    , engineElo_(engineElo)
    , engineIsWhite_(engineIsWhite)
{
    setStyleSheet("MenuWindow { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #2c3e50, stop: 1 #34495e); }");

    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    mainLayout->addWidget(board_, 3);

    auto *sideLayout = new QVBoxLayout();
    sideLayout->setSpacing(10);

    historyList_->setStyleSheet(
        "QListWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 1px solid #bdc3c7;"
        "   border-radius: 5px;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 11pt;"
        "}"
        "QListWidget::item {"
        "   padding: 5px;"
        "   border-bottom: 1px solid #ecf0f1;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #3498db;"
        "   color: white;"
        "}"
        );

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #3498db;"
        "   border: none;"
        "   color: white;"
        "   padding: 10px;"
        "   font-size: 12pt;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #21618c;"
        "}";

    resignButton_->setStyleSheet(buttonStyle);
    returnToMenuButton_->setStyleSheet(buttonStyle);

    sideLayout->addWidget(historyList_);
    sideLayout->addWidget(resignButton_);
    sideLayout->addWidget(returnToMenuButton_);

    mainLayout->addLayout(sideLayout, 1);

    connect(resignButton_, &QPushButton::clicked, this, &MenuWindow::onResign);
    connect(returnToMenuButton_, &QPushButton::clicked, this, &MenuWindow::onReturnToMenu);
    connect(board_, &ChessBoardWidget::moveMade, this, &MenuWindow::onMoveMade);
    connect(board_, &ChessBoardWidget::gameReset, this, [this]() {
        historyList_->clear();
        halfmoveCount_ = 0;
    });

    if (vsEngine_) {
        board_->setPlayVsEngine(true, engineIsWhite_, engineElo_);
    } else {
        board_->setPlayVsEngine(false, false, engineElo_);
    }
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
