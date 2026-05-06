// Autori: xdurec00
// hlavne okno

#pragma once
#include <QMainWindow>
#include <memory>
#include "petrinet.hpp"

class PetriNet;
class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onOpen();

private:
    void loadNet(const QString &path);
    void buildScene();

    QGraphicsScene *m_scene;
    QGraphicsView  *m_view;
    std::shared_ptr<PetriNet> m_net;
};