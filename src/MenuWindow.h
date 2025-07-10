#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include "ChessBoardWidget.h"
#include "MainMenuWidget.h"

class MenuWindow : public QWidget {
    Q_OBJECT
public:
    explicit MenuWindow(QWidget* parent = nullptr);
    ~MenuWindow() override = default;

private slots:
    void onResign();
    void onMoveMade(const QString& san);
    void onReturnToMenu();

private:
    ChessBoardWidget* board_;
    QListWidget* historyList_;
    QPushButton* resignButton_;
    QPushButton* returnToMenuButton_;
    int halfmoveCount_;
};

#endif // MENUWINDOW_H