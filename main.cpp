#include <QApplication>
#include "src/ChessBoardWidget.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ChessBoardWidget widget;
    widget.newGame();
    widget.show();

    return app.exec();
}
