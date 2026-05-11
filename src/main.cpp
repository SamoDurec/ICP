// Autori: xdurecs00, xpertod00
// Hlavny vstupny bod

#include <QApplication>
#include "mainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}