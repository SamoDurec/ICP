/**
 * @file mainWindow.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie hlavneho okna.
 */

#include "mainWindow.hpp"
#include "placeItem.hpp"
#include "transitionItem.hpp"
#include "arcItem.hpp"
#include "parser.hpp"
#include "netRunner.hpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox> 
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMap>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>

/**
 * @brief Hlavna metoda zaistujuca vykreslenie sceny.
 * Vykresli do okna vsetky polozky, a nacita siet definovanu v subore examples/test.pn.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ICP Petri Net");
    resize(1000, 700);

    // scena
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-1000, -1000, 2000, 2000);
    m_view = new QGraphicsView(m_scene, this);
    m_view->viewport()->installEventFilter(this);
    m_view->setRenderHint(QPainter::Antialiasing);

    // log
    m_log = new QPlainTextEdit(this);
    m_log->setReadOnly(true);
    m_log->setMaximumHeight(150);
    m_log->setFont(QFont("Courier", 9));

    // inject panel
    m_injectName  = new QLineEdit(this);
    m_injectName->setPlaceholderText("Vstup (napr. in)");
    m_injectValue = new QLineEdit(this);
    m_injectValue->setPlaceholderText("Hodnota (napr. 1)");
    auto *injectBtn = new QPushButton("Inject", this);
    connect(injectBtn, &QPushButton::clicked, this, &MainWindow::onInject);

    auto *injectLayout = new QHBoxLayout();
    injectLayout->addWidget(new QLabel("Vstup:"));
    injectLayout->addWidget(m_injectName);
    injectLayout->addWidget(new QLabel("Hodnota:"));
    injectLayout->addWidget(m_injectValue);
    injectLayout->addWidget(injectBtn);

    auto *injectWidget = new QWidget(this);
    injectWidget->setLayout(injectLayout);

    // dole panel - log a inject
    auto *bottomWidget = new QWidget(this);
    auto *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->addWidget(injectWidget);
    bottomLayout->addWidget(m_log);

    // splitter
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(m_view);
    splitter->addWidget(bottomWidget);
    splitter->setStretchFactor(0, 1);
    setCentralWidget(splitter);

    // menu
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Open...", this, &MainWindow::onOpen);
    fileMenu->addAction("Save", this, &MainWindow::onSave);

    QMenu *runMenu = menuBar()->addMenu("Run");
    runMenu->addAction("Start", this, &MainWindow::onStart);
    runMenu->addAction("Stop",  this, &MainWindow::onStop);

    QMenu *modifyMenu = menuBar()->addMenu("Modify");
    modifyMenu->addAction("Add place", this, &MainWindow::onAddPlace);
    modifyMenu->addAction("Delete place", this, &MainWindow::onDeletePlace);
    modifyMenu->addAction("Add transition", this, &MainWindow::onAddTransition);
    modifyMenu->addAction("Delete transition", this, &MainWindow::onDeleteTransition);
    modifyMenu->addAction("Add arc", this, &MainWindow::onAddArc);
    modifyMenu->addAction("Delete arc", this, &MainWindow::onDeleteArc);

    loadNet("examples/test.pn");
}

/**
 * @brief Otvori a nacita Petriho siet z vybraneho suboru.
 */
void MainWindow::onOpen() {
    QString path = QFileDialog::getOpenFileName(
        this, "Open Petri Net", "", "Petri Net (*.pn);;All files (*)");
    if (!path.isEmpty()) {
        loadNet(path);
    }
}

/**
 * @brief Ulozi Petriho siet, ktora je aktualne na obrazovke do vybraneho alebo noveho súboru.
 */
void MainWindow::onSave() {
    // najdenie suboru
    QString path = QFileDialog::getSaveFileName(
        this, "Save Petri Net", "", "Petri Net (*.pn);;All files (*)");
    
    if (path.isEmpty()) {
        return;
    }

    QFile file(path); // nastavenie cesty

    // otvorenie súboru
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file";
        return;
    }

    // zapis do suboru
    QTextStream out(&file);

    // meno
    out << QString::fromUtf8("Jméno sítě:\n");
    out << "\t" << m_net->name() << "\n";    

    // komentar
    out << QString::fromUtf8("Komentář:\n");
    out << "\t" << m_net->comment() << "\n";

    // vstupy
    out << QString::fromUtf8("Vstupy:\n");
    for (const auto &i : m_net->inputs()) {
        out << "\t"
            << i
            << "\n";
    }

    // vystupy
    out << QString::fromUtf8("Výstupy:\n");
    for (const auto &o : m_net->outputs()) {
        out << "\t"
            << o
            << "\n";
    }

    // premenne
    out << QString::fromUtf8("Proměnné:\n");
    for (const auto &v : m_net->variables()) {
        out << "\t"
            << v
            << "\n";
    }

    // miesta
    out << QString::fromUtf8("Místa:\n");
    for (const auto &p : m_net->places()) {
        out << "\t"
            << p->id()
            << " "
            << "("
            << p->initialTokens()
            << ")"
            << "\n";
    }

    // prechody
    out << QString::fromUtf8("Přechody:\n");
    for (const auto &t : m_net->transitions()) {
        out << t->id()
            << " :\n";

        out << "\tin: ";
        for (const Arc &a : t->inputArcs()) {
            out << a.placeId
                << "*"
                << a.weight
                << " ";
        }

        out << "\n\tout: ";
        for (const Arc &a : t->outputArcs()) {
            out << a.placeId
                << "*"
                << a.weight
                << " ";
        }

        out << "\n\twhen: "
            << t->eventName()
            << " [ "
            << t->guard()
            << " ]";
        if (t->isDelayed()) {
            out << " @ "
                << t->delayMs();
        }

        out << "\n\tdo: { "
            << t->action()
            << " }\n";
        
    }

    // zatvorenie suboru
    file.close();

    qDebug() << "Saved to: " << path; // log

}

void MainWindow::onStart() {
    if (!m_net) {
        return;
    }
    if (!m_runner) {
        m_runner = new NetRunner(m_net, this);
        connect(m_runner, &NetRunner::logMessage,
                this, &MainWindow::onLogMessage);
        connect(m_runner, &NetRunner::markingChanged,
                this, &MainWindow::onMarkingChanged);
    }
    m_runner->start();
}

void MainWindow::onStop() {
    if (m_runner) {
        m_runner->stop();
    }
}

/**
 * @brief Vlozenie miesta.
 */
void MainWindow::onAddPlace() {
    auto p = m_net->addPlace("UNNAMED", 0);
    p->setPos(QPointF(0, 200));

    buildScene();
}

/**
 * @brief Zmazanie zakliknuteho miesta.
 */
void MainWindow::onDeletePlace() {
    auto selected = m_scene->selectedItems();

    // prechadza vsetky polozky, kym nanajde zakliknutu
    for(auto *item : selected) {
        auto *placeItem = dynamic_cast<PlaceItem*>(item);

        if(!placeItem) continue;

        QString id = placeItem->place()->id();

        m_net->removePlace(id);
    }

    buildScene();
}

/**
 * @brief Vlozenie prechodu.
 */
void MainWindow::onAddTransition()
{
    auto t = m_net->addTransition("Unnamed");
    t->setPos(QPointF(100, 200));

    buildScene();
}

/**
 * @brief Zmazanie zakliknutého prechodu.
 */
void MainWindow::onDeleteTransition()
{
    auto selected = m_scene->selectedItems();

    // prechadza vsetky polozky, kým nenájde zakliknutú
    for (auto *item : selected)
    {
        auto *tItem = dynamic_cast<TransitionItem*>(item);

        if (!tItem) continue;

        QString id = tItem->transition()->id();

        m_net->removeTransition(id);
    }

    buildScene();
}

/**
 * @brief Aktivuje mod pridanie hrany.
 */
void MainWindow::onAddArc() {
    m_addArcMode = true;
    m_arcStart = nullptr;

    m_log->appendPlainText("Click source and target");
}

/**
 * @brief Zaistuje pridanie hrany.
 * Prebehne len ak je m_addArcMode true.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (!m_addArcMode) {
        return false;
    }
    if (obj != m_view->viewport()) {
        return false;
    }
    if (event->type() != QEvent::MouseButtonPress) {
        return false;
    }

    // ulozenie pozicii zakliknutych objektov
    auto *me = static_cast<QMouseEvent*>(event);
    QPointF scenePos = m_view->mapToScene(me->pos());
    QGraphicsItem *clicked = m_scene->itemAt(scenePos, QTransform());

    if (!clicked) {
        return false;
    }

    // ignoruj ArcItem
    if (dynamic_cast<ArcItem*>(clicked)) {
        return false;
    }

    if (!m_arcStart) {
        m_arcStart = clicked;
        m_log->appendPlainText("Source selected, click target");
        return true;
    }

    // ak bol zakliknuty dvakrat rovnaky objekt skonci
    if (clicked == m_arcStart) {
        return true;
    }

    // rozlisenie typov objektov
    auto *p1 = dynamic_cast<PlaceItem*>(m_arcStart);
    auto *t1 = dynamic_cast<TransitionItem*>(m_arcStart);
    auto *p2 = dynamic_cast<PlaceItem*>(clicked);
    auto *t2 = dynamic_cast<TransitionItem*>(clicked);

    // kontrola pozicie hrany a vykreslenia
    if (p1 && t2) {
        Arc a; a.placeId = p1->place()->id(); a.weight = 1;
        t2->transition()->addInputArc(a);
        m_log->appendPlainText("Arc added");
    } else if (t1 && p2) {
        Arc a; a.placeId = p2->place()->id(); a.weight = 1;
        t1->transition()->addOutputArc(a);
        m_log->appendPlainText("Arc added");
    } else {
        m_log->appendPlainText("Invalid arc (place->place or transition->transition)");
    }

    m_addArcMode = false;
    m_arcStart = nullptr;
    buildScene();
    return true;
}

/**
 * @brief Zmazanie zakliknutej hrany.
 */
void MainWindow::onDeleteArc() {
    auto selected = m_scene->selectedItems();

    if(selected.isEmpty()) {
        return;
    }
    auto *arcItem = dynamic_cast<ArcItem*>(selected.first());

    // log
    if(!arcItem) {
        m_log->appendPlainText("Select arc");
        return;
    }

    // ulozenie a zaciatku a konca hrany
    auto *from = arcItem->fromItem();
    auto *to = arcItem->toItem();

    // rozlisenie typov objektov
    auto *p1 = dynamic_cast<PlaceItem*>(from);
    auto *t1 = dynamic_cast<TransitionItem*>(from);
    auto *p2 = dynamic_cast<PlaceItem*>(to);
    auto *t2 = dynamic_cast<TransitionItem*>(to);

    // zmazanie hrany z prechodu
    if (p1 && t2) { // place -> transition
        QString pid = p1->place()->id();
        QVector<Arc> arcs;

        for (const auto &a : t2->transition()->inputArcs()) {
            if (a.placeId != pid) {
                arcs.append(a);
            }
        }

        t2->transition()->setInputArcs(arcs);
    } else if (t1 && p2) { // transition -> place
        QString pid = p2->place()->id();
        QVector<Arc> arcs;

        for (const auto &a : t1->transition()->outputArcs()) {
            if (a.placeId != pid) {
                arcs.append(a);
            }
        }

        t1->transition()->setOutputArcs(arcs);
    } 

    buildScene();
}

void MainWindow::onInject() {
    if (!m_runner || !m_runner->isRunning()) {
        m_log->appendPlainText("! Siet nebezi - najprv stlac Run->Start");
        return;
    }
    QString name  = m_injectName->text().trimmed();
    QString value = m_injectValue->text().trimmed();
    if (name.isEmpty()) {
        return;
    }
    m_runner->injectInput(name, value);
}

void MainWindow::onLogMessage(const QString &msg) {
    m_log->appendPlainText(msg);
}

void MainWindow::onMarkingChanged() {
    for (auto *item : m_scene->items()) {
        if (auto *pi = dynamic_cast<PlaceItem*>(item)) {
            pi->refresh();
        }

        if (auto *ti = dynamic_cast<TransitionItem*>(item)) {
            auto t = ti->transition();
            if (m_runner && m_runner->isPendingTimer(t->id())) {
                ti->setState(TransitionItem::State::PendingTimer);
            } else if (m_net->isEnabled(t)) {
                ti->setState(TransitionItem::State::Enabled);
            } else {
                ti->setState(TransitionItem::State::Normal);
            }
        }
    }
}

void MainWindow::loadNet(const QString &path) {
    if (m_runner) { 
        m_runner->stop(); 
        delete m_runner; 
        m_runner = nullptr; 
    }
    QString error;
    m_net = Parser::load(path, error);
    if (!m_net) { 
        QMessageBox::critical(this, "Chyba", error); 
        return; 
    }
    setWindowTitle("ICP Petri Net — " + m_net->name());
    buildScene();
}

void MainWindow::buildScene() {
    m_scene->blockSignals(true);
    m_scene->clear();
    if (!m_net) {
        m_scene->blockSignals(false);
        return;
    }
    QMap<QString, QGraphicsItem*> items;

    int x = -200;
    for (auto &place : m_net->places()) {
        auto *item = new PlaceItem(place);
        if (place->pos() == QPointF(0, 0)) {
            place->setPos(QPointF(x, -80));
        }

        item->setPos(place->pos());
        m_scene->addItem(item);
        items[place->id()] = item;
        x += 120;
    }

    x = -140;
    for (auto &t : m_net->transitions()) {
        auto *item = new TransitionItem(t);
        if (t->pos() == QPointF(0, 0)) {
            t->setPos(QPointF(x, 80));
        }
        item->setPos(t->pos());
        m_scene->addItem(item);
        items[t->id()] = item;
        x += 120;
    }

    for (auto &t : m_net->transitions()) {
        QGraphicsItem *tItem = items[t->id()];
        if (!tItem) {
            continue;
        }
        for (const auto &arc : t->inputArcs()) {
            QGraphicsItem *pItem = items[arc.placeId];
            if (pItem) {
                ArcItem *line = new ArcItem(pItem, tItem, arc.weight);

                m_scene->addItem(line);

                auto *placeItem = dynamic_cast<PlaceItem*>(pItem);
                auto *transitionItem = dynamic_cast<TransitionItem*>(tItem);

                if (placeItem) {
                    placeItem->addArc(line);
                }
                if (transitionItem) {
                    transitionItem->addArc(line);
                }
            }
        }

        for (const auto &arc : t->outputArcs()) {
            QGraphicsItem *pItem = items[arc.placeId];
            if (pItem) {
                ArcItem *line = new ArcItem(tItem, pItem, arc.weight);

                m_scene->addItem(line);

                auto *placeItem = dynamic_cast<PlaceItem*>(pItem);
                auto *transitionItem = dynamic_cast<TransitionItem*>(tItem);

                if (placeItem) {
                    placeItem->addArc(line);
                }
                if (transitionItem) {
                    transitionItem->addArc(line);
                }
            }
        }
    }
    m_scene->blockSignals(false);
}