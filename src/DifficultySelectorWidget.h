#ifndef DIFFICULTYSELECTORWIDGET_H
#define DIFFICULTYSELECTORWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>

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

    QButtonGroup* colorGroup_{};
    QRadioButton* whiteBtn_{};
    QRadioButton* blackBtn_{};
    QRadioButton* randomBtn_{};

    bool decideEngineIsWhite() const;
};

#endif // DIFFICULTYSELECTORWIDGET_H
