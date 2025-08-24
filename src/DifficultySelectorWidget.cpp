#include "DifficultySelectorWidget.h"
#include "MenuWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

DifficultySelectorWidget::DifficultySelectorWidget(QWidget* parent)
    : QWidget(parent) {

    easyButton_ = new QPushButton("Лёгкий", this);
    mediumButton_ = new QPushButton("Средний", this);
    hardButton_ = new QPushButton("Сложный", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(easyButton_);
    layout->addWidget(mediumButton_);
    layout->addWidget(hardButton_);

    setLayout(layout);
    setWindowTitle("Выбор сложности");
    resize(250, 150);

    connect(easyButton_, &QPushButton::clicked, this, &DifficultySelectorWidget::onEasyClicked);
    connect(mediumButton_, &QPushButton::clicked, this, &DifficultySelectorWidget::onMediumClicked);
    connect(hardButton_, &QPushButton::clicked, this, &DifficultySelectorWidget::onHardClicked);
}

void DifficultySelectorWidget::onEasyClicked() {
    QMessageBox::information(this, "Бот", "Игра с ботом (легкий уровень)");
    auto* window = new MenuWindow(true, 1200, false);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    close();
}

void DifficultySelectorWidget::onMediumClicked() {
    QMessageBox::information(this, "Бот", "Игра с ботом (средний уровень)");
    auto* window = new MenuWindow(true, 1600, false);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    close();
}

void DifficultySelectorWidget::onHardClicked() {
    QMessageBox::information(this, "Бот", "Игра с ботом (сложный уровень)");
    auto* window = new MenuWindow(true, 2000, false);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    close();
}
