// Autori: xdurec00
// hlavne okno

#pragma once
#include <QMainWindow>
#include <memory>

class PetriNet;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QGraphicsScene *m_scene;
    QGraphicsView  *m_view;
};