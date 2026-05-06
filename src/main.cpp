// Autori: xdurecs00, x
// Hlavny vstupny bod aplikacie
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include "parser.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // TEST: nacitaj siet
    QString error;
    auto net = Parser::load("examples/test.pn", error);
    if (!net) {
        qDebug() << "CHYBA:" << error;
    } else {
        qDebug() << "=== Siet nacitana ===";
        qDebug() << "Meno:" << net->name();
        qDebug() << "Komentar:" << net->comment();
        qDebug() << "Vstupy:" << net->inputs();
        qDebug() << "Vystupy:" << net->outputs();
        qDebug() << "Miesta:";
        for (auto &p : net->places())
            qDebug() << " " << p->id() << "tokeny:" << p->tokens();
        qDebug() << "Prechody:";
        for (auto &t : net->transitions())
            qDebug() << " " << t->id() << "event:" << t->eventName();
    }

    QMainWindow w;
    w.setWindowTitle("ICP Petri Net");
    w.resize(800, 600);
    w.setCentralWidget(new QLabel("Hello Petri Net!", &w));
    w.show();
    return app.exec();
}