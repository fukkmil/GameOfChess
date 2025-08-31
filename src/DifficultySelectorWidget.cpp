#include "DifficultySelectorWidget.h"
#include "MenuWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QString>
#include <QRandomGenerator>

DifficultySelectorWidget::DifficultySelectorWidget(QWidget* parent)
    : QWidget(parent) {
    auto* colorLabel = new QLabel("Цвет:", this);
    whiteBtn_  = new QRadioButton("Я за белых", this);
    blackBtn_  = new QRadioButton("Я за чёрных", this);
    randomBtn_ = new QRadioButton("Случайно", this);
    randomBtn_->setChecked(true);

    colorGroup_ = new QButtonGroup(this);
    colorGroup_->addButton(whiteBtn_,  0);
    colorGroup_->addButton(blackBtn_,  1);
    colorGroup_->addButton(randomBtn_, 2);

    auto* colorRow = new QHBoxLayout();
    colorRow->addWidget(colorLabel);
    colorRow->addSpacing(8);
    colorRow->addWidget(whiteBtn_);
    colorRow->addWidget(blackBtn_);
    colorRow->addWidget(randomBtn_);
    colorRow->addStretch();

    easyButton_ = new QPushButton("Лёгкий", this);
    mediumButton_ = new QPushButton("Средний", this);
    hardButton_ = new QPushButton("Сложный", this);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(colorRow);
    layout->addSpacing(6);
    layout->addWidget(easyButton_);
    layout->addWidget(mediumButton_);
    layout->addWidget(hardButton_);
    layout->addStretch();

    setLayout(layout);
    setWindowTitle("Выбор сложности");
    resize(250, 150);

    connect(easyButton_, &QPushButton::clicked, this, &DifficultySelectorWidget::onEasyClicked);
    connect(mediumButton_, &QPushButton::clicked, this, &DifficultySelectorWidget::onMediumClicked);
    connect(hardButton_, &QPushButton::clicked, this, &DifficultySelectorWidget::onHardClicked);
}

void DifficultySelectorWidget::onEasyClicked() {
    const bool engineWhite = decideEngineIsWhite();
    const QString color = (engineWhite ? "чёрными" : "белыми");
    QMessageBox::information(this, "Бот", "Игра с ботом (легкий уровень), вы играете " + color);
    auto* window = new MenuWindow(true, 1200, engineWhite);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    close();
}

void DifficultySelectorWidget::onMediumClicked() {
    const bool engineWhite = decideEngineIsWhite();
    const QString color = (engineWhite ? "чёрными" : "белыми");
    QMessageBox::information(this, "Бот", "Игра с ботом (легкий уровень), вы играете " + color);
    auto* window = new MenuWindow(true, 1600, engineWhite);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    close();
}

void DifficultySelectorWidget::onHardClicked() {
    const bool engineWhite = decideEngineIsWhite();
    const QString color = (engineWhite ? "чёрными" : "белыми");
    QMessageBox::information(this, "Бот", "Игра с ботом (легкий уровень), вы играете " + color);
    auto* window = new MenuWindow(true, 2000,engineWhite);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    close();
}

bool DifficultySelectorWidget::decideEngineIsWhite() const {
    const int id = colorGroup_->checkedId();
    switch (id) {
        case 0:
            return false;
        case 1:
            return true;
        case 2: default: {
            const bool engineWhite = (QRandomGenerator::global()->bounded(2) == 1);
            return engineWhite;
        }
    }
}