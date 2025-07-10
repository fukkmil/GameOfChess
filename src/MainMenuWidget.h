#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "DifficultySelectorWidget.h"

class MainMenuWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainMenuWidget(QWidget* parent = nullptr);

private slots:
    void onPlayWithComputerClicked();
    void onPlayWithHumanClicked();

private:
    QPushButton* playWithComputerButton_;
    QPushButton* playWithHumanButton_;
    DifficultySelectorWidget* difficultyWidget_;
};

#endif // MAINMENUWIDGET_H
