// Autori: xdurec00
// implementacia hlavneho okna

#include "mainWindow.hpp"
#include "placeItem.hpp"
#include "transitionItem.hpp"
#include "arcItem.hpp"
#include "parser.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ICP Petri Net");
    resize(1000, 700);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-1000, -1000, 2000, 2000);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(m_view);

    // Menu
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Open...", this, &MainWindow::onOpen);

    // Nacitaj default siet
    loadNet("examples/test.pn");
}

void MainWindow::onOpen()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open Petri Net", "", "Petri Net (*.pn);;All files (*)");
    qDebug() << "Vybrany subor:" << path;
    if (!path.isEmpty())
        loadNet(path);
}

void MainWindow::loadNet(const QString &path)
{
    QString error;
    m_net = Parser::load(path, error);
    if (!m_net) {
        QMessageBox::critical(this, "Chyba", error);
        return;
    }
    setWindowTitle("ICP Petri Net — " + m_net->name());
    buildScene();
}

void MainWindow::buildScene()
{
    m_scene->clear();
    if (!m_net) return;

    QMap<QString, QGraphicsItem*> items;

    // Miesta
    int x = -200;
    for (auto &place : m_net->places()) {
        auto *item = new PlaceItem(place);
        item->setPos(x, -80);
        m_scene->addItem(item);
        items[place->id()] = item;
        x += 120;
    }

    // Prechody
    x = -140;
    for (auto &t : m_net->transitions()) {
        auto *item = new TransitionItem(t);
        item->setPos(x, 80);
        m_scene->addItem(item);
        items[t->id()] = item;
        x += 120;
    }

    // Hrany
    for (auto &t : m_net->transitions()) {
        QGraphicsItem *tItem = items[t->id()];
        if (!tItem) continue;
        for (const auto &arc : t->inputArcs()) {
            QGraphicsItem *pItem = items[arc.placeId];
            if (pItem) m_scene->addItem(new ArcItem(pItem, tItem, arc.weight));
        }
        for (const auto &arc : t->outputArcs()) {
            QGraphicsItem *pItem = items[arc.placeId];
            if (pItem) m_scene->addItem(new ArcItem(tItem, pItem, arc.weight));
        }
    }
}