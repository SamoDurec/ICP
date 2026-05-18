/**
 * @file main.cpp
 * @authors xdurecs00, xpertod00
 * @brief Vstupny bod aplikacie
 */

#include <QApplication>
#include "mainWindow.hpp"

/**
 * @brief Vstupny bod aplikacie, spusti aplikaciu a hlavne okno.
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}