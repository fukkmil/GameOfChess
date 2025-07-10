#include <QApplication>
#include "src/MenuWindow.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MenuWindow menu;
    menu.show();

    return QApplication::exec();
}
