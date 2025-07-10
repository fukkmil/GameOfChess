#include <QApplication>
#include "src/MainMenuWidget.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainMenuWidget mainmenu;
    mainmenu.show();


    return QApplication::exec();
}
