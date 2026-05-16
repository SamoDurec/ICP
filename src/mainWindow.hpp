/**
 * @file mainWindow.hpp
 * @authors xdurecs00, xpertod00
 * @brief Hlavne okno aplikacie.
 */

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

/**
 * @brief Hlavne okno aplikacie — editor a monitor Petriho siete.
 *
 * Obsahuje graficku scenu, log panel, inject panel a menu.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /** @brief Konstruktor hlavneho okna. */
    explicit MainWindow(QWidget *parent = nullptr);

    QGraphicsItem *m_arcStart = nullptr; ///< Startovaci prvok pri pridavani hrany
    bool m_addArcMode = false;           ///< True ak je aktivny rezim pridavania hrany

private slots:
    /** @brief Otvori dialog pre nacitanie siete. */
    void onOpen();
    /** @brief Otvori dialog pre ulozenie siete. */
    void onSave();
    /** @brief Spusti siet. */
    void onStart();
    /** @brief Zastavi siet. */
    void onStop();
    /** @brief Prida nove miesto. */
    void onAddPlace();
    /** @brief Odstrani vybrate miesto. */
    void onDeletePlace();
    /** @brief Prida novy prechod. */
    void onAddTransition();
    /** @brief Odstrani vybrany prechod. */
    void onDeleteTransition();
    /** @brief Aktivuje rezim pridavania hrany. */
    void onAddArc();
    /** @brief Odstrani vybranu hranu. */
    void onDeleteArc();
    /** @brief Zapise spravu do log panelu. */
    void onLogMessage(const QString &msg);
    /** @brief Prekreslí tokeny a farby po zmene markingu. */
    void onMarkingChanged();
    /** @brief Injektuje vstup do beziace siete. */
    void onInject();

private:
    /** @brief Nacita siet zo suboru a prekresli scenu. */
    void loadNet(const QString &path);
    /** @brief Prekresli celu graficku scenu. */
    void buildScene();
    /** @brief Spracuje kliknutia mysi pri pridavani hrany. */
    bool eventFilter(QObject *obj, QEvent *event) override;

    QGraphicsScene            *m_scene;          ///< Graficka scena
    QGraphicsView             *m_view;            ///< Zobrazovac sceny
    QPlainTextEdit            *m_log;             ///< Log panel
    QLineEdit                 *m_injectName;      ///< Pole pre meno vstupu
    QLineEdit                 *m_injectValue;     ///< Pole pre hodnotu vstupu
    std::shared_ptr<PetriNet>  m_net;             ///< Aktualna siet
    NetRunner                 *m_runner {nullptr}; ///< Runtime interpreter
};