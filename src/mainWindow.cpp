// Autori: xdurec00
// implementacia hlavneho okna

#include "mainWindow.hpp"
#include "placeItem.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>

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

    // zelena gula v okne
    auto place = std::make_shared<Place>("IDLE", 1);
    auto *item = new PlaceItem(place);
    item->setPos(0, 0);
    m_scene->addItem(item);
}