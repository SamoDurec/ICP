// Autori: xdurec00
// hlavne okno

#pragma once
#include <QMainWindow>
#include <memory>
#include "petrinet.hpp"

class QGraphicsScene;
class QGraphicsView;
class QPlainTextEdit;
class QLineEdit;
class NetRunner;
class QGraphicsItem;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    QGraphicsItem *m_arcStart = nullptr;
    bool m_addArcMode = false;

private slots:
    void onOpen();
    void onSave();
    void onStart();
    void onStop();
    void onAddPlace();
    void onDeletePlace();
    void onAddTransition();
    void onSceneSelectionChanged();
    void onDeleteTransition();
    void onAddArc();
    void onDeleteArc();
    void onLogMessage(const QString &msg);
    void onMarkingChanged();
    void onInject();

private:
    void loadNet(const QString &path);
    void buildScene();

    QGraphicsScene            *m_scene;
    QGraphicsView             *m_view;
    QPlainTextEdit            *m_log;
    QLineEdit                 *m_injectName;
    QLineEdit                 *m_injectValue;
    std::shared_ptr<PetriNet>  m_net;
    NetRunner                 *m_runner {nullptr};
};