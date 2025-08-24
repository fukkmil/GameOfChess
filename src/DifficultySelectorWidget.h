#ifndef DIFFICULTYSELECTORWIDGET_H
#define DIFFICULTYSELECTORWIDGET_H

#include <QWidget>
#include <QPushButton>

class DifficultySelectorWidget : public QWidget {
    Q_OBJECT

public:
    explicit DifficultySelectorWidget(QWidget* parent = nullptr);

private slots:
    void onEasyClicked();
    void onMediumClicked();
    void onHardClicked();

private:
    QPushButton* easyButton_;
    QPushButton* mediumButton_;
    QPushButton* hardButton_;
};

#endif // DIFFICULTYSELECTORWIDGET_H
