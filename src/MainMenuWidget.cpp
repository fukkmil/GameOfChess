#include "MainMenuWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include "MenuWindow.h"

MainMenuWidget::MainMenuWidget(QWidget* parent) : QWidget(parent) {
    setStyleSheet("MainMenuWidget { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #1a2980, stop: 1 #26d0ce); }");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(50, 50, 50, 50);

    QLabel* titleLabel = new QLabel("GameOfChess", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 36pt;"
        "   font-weight: bold;"
        "   margin: 20px;"
        "   background: transparent;"
        "}"
        );
    layout->addWidget(titleLabel);

    layout->addStretch(1);

    playWithComputerButton_ = new QPushButton("Против компьютера", this);
    playWithHumanButton_ = new QPushButton("Против человека", this);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #ffffff;"
        "   color: #1a2980;"
        "   padding: 15px 20px;"
        "   font-size: 12pt;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   min-width: 180px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 230);"
        "   border: 2px solid #26d0ce;"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255, 255, 255, 150);"
        "}";

    playWithComputerButton_->setStyleSheet(buttonStyle);
    playWithHumanButton_->setStyleSheet(buttonStyle);

    playWithComputerButton_->setFixedSize(220, 60);
    playWithHumanButton_->setFixedSize(220, 60);

    QHBoxLayout* computerLayout = new QHBoxLayout();
    computerLayout->addStretch();
    computerLayout->addWidget(playWithComputerButton_);
    computerLayout->addStretch();

    QHBoxLayout* humanLayout = new QHBoxLayout();
    humanLayout->addStretch();
    humanLayout->addWidget(playWithHumanButton_);
    humanLayout->addStretch();

    layout->addLayout(computerLayout);
    layout->addLayout(humanLayout);

    layout->addStretch(1);

    connect(playWithComputerButton_, &QPushButton::clicked, this, &MainMenuWidget::onPlayWithComputerClicked);
    connect(playWithHumanButton_, &QPushButton::clicked, this, &MainMenuWidget::onPlayWithHumanClicked);
}

void MainMenuWidget::onPlayWithComputerClicked() {
    close();
    auto* window = new MenuWindow(true, 1600, false);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainMenuWidget::onPlayWithHumanClicked() {
    close();
    auto* window = new MenuWindow(false, 1600, false);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}
