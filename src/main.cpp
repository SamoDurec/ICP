/**
 * @file main.cpp
 * @authors xdurecs00, xpertod00
 * @brief Vstupny bod aplikacie
 */

#include <QApplication>
#include "mainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}