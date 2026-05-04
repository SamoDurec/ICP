// Autori: xdurecs00, x
// Hlavny vstupny bod aplikacie
#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow w;
    w.setWindowTitle("ICP Petri Net");
    w.resize(800, 600);
    w.setCentralWidget(new QLabel("Hello Petri Net!", &w));
    w.show();
    return app.exec();
}