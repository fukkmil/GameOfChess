#include "MainMenuWidget.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "MenuWindow.h"

MainMenuWidget::MainMenuWidget(QWidget* parent)
    : QWidget(parent), difficultyWidget_(nullptr) {

    playWithComputerButton_ = new QPushButton("Играть с компьютером", this);
    playWithHumanButton_ = new QPushButton("Играть с человеком", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(playWithComputerButton_);
    layout->addWidget(playWithHumanButton_);

    setLayout(layout);
    setWindowTitle("Шахматы — Главное меню");
    resize(300, 200);

    connect(playWithComputerButton_, &QPushButton::clicked, this, &MainMenuWidget::onPlayWithComputerClicked);
    connect(playWithHumanButton_, &QPushButton::clicked, this, &MainMenuWidget::onPlayWithHumanClicked);
}

void MainMenuWidget::onPlayWithComputerClicked() {
    close();
    difficultyWidget_ = new DifficultySelectorWidget();
    difficultyWidget_->show();
}

void MainMenuWidget::onPlayWithHumanClicked() {
    close();
    auto* window = new MenuWindow(false,1600,false);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}
