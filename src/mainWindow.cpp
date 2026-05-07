// Autori: xdurec00, xpertod00
// implementacia hlavneho okna

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ICP Petri Net");
    resize(1000, 700);

    // scena
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-1000, -1000, 2000, 2000);
    m_view = new QGraphicsView(m_scene, this);
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

    loadNet("examples/test.pn");
}

void MainWindow::onOpen()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open Petri Net", "", "Petri Net (*.pn);;All files (*)");
    if (!path.isEmpty()) loadNet(path);
}

void MainWindow::onSave()
{
    // nalezeni suboru
    QString path = QFileDialog::getSaveFileName(
        this, "Save Petri Net", "", "Petri Net (*.pn);;All files (*)");
    
    if (path.isEmpty()) return;

    QFile file(path); // nastaveni cesty

    // otevrenin suboru
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file";
        return;
    }

    // Zapis do suboru
    QTextStream out(&file);

    out << QString::fromUtf8("Jméno sítě:\n");
    out << "\t" << m_net->name() << "\n";    

    out << QString::fromUtf8("Komentář:\n");
    out << "\t" << m_net->comment() << "\n";

    out << QString::fromUtf8("Vstupy:\n");
    for (const auto &i : m_net->inputs())
    {
        out << "\t"
            << i
            << "\n";
    }

    out << QString::fromUtf8("Výstupy:\n");
    for (const auto &o : m_net->outputs())
    {
        out << "\t"
            << o
            << "\n";
    }

    out << QString::fromUtf8("Proměnné:\n");
    for (const auto &v : m_net->variables())
    {
        out << "\t"
            << v
            << "\n";
    }

    out << QString::fromUtf8("Místa:\n");
    for (const auto &p : m_net->places())
    {
        out << "\t"
            << p->id()
            << " "
            << "("
            << p->tokens()
            << ")"
            << "\n";
    }

    out << QString::fromUtf8("Přechody:\n");
    for (const auto &t : m_net->transitions())
    {
        out << t->id()
            << " :\n";

        out << "\tin: ";
        for (const Arc &a : t->inputArcs())
        {
            out << a.placeId
                << "*"
                << a.weight
                << " ";
        }

        out << "\n\tout: ";
        for (const Arc &a : t->outputArcs())
        {
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
        if (t->isDelayed())
        {
            out << " @ "
                << t->delayMs();
        }

        out << "\n\tdo: { "
            << t->action()
            << " }";
        
    }

    // zavreni suboru
    file.close();

    qDebug() << "Saved to: " << path;

}

void MainWindow::onStart()
{
    if (!m_net) return;
    if (!m_runner) {
        m_runner = new NetRunner(m_net, this);
        connect(m_runner, &NetRunner::logMessage,
                this, &MainWindow::onLogMessage);
        connect(m_runner, &NetRunner::markingChanged,
                this, &MainWindow::onMarkingChanged);
    }
    m_runner->start();
}

void MainWindow::onStop()
{
    if (m_runner) m_runner->stop();
}

void MainWindow::onInject()
{
    if (!m_runner || !m_runner->isRunning()) {
        m_log->appendPlainText("! Siet nebezi - najprv stlac Run->Start");
        return;
    }
    QString name  = m_injectName->text().trimmed();
    QString value = m_injectValue->text().trimmed();
    if (name.isEmpty()) return;
    m_runner->injectInput(name, value);
}

void MainWindow::onLogMessage(const QString &msg)
{
    m_log->appendPlainText(msg);
}

void MainWindow::onMarkingChanged()
{
    for (auto *item : m_scene->items()) {
        if (auto *pi = dynamic_cast<PlaceItem*>(item))
            pi->refresh();
    }
}

void MainWindow::loadNet(const QString &path)
{
    if (m_runner) { m_runner->stop(); delete m_runner; m_runner = nullptr; }
    QString error;
    m_net = Parser::load(path, error);
    if (!m_net) { QMessageBox::critical(this, "Chyba", error); return; }
    setWindowTitle("ICP Petri Net — " + m_net->name());
    buildScene();
}

void MainWindow::buildScene()
{
    m_scene->clear();
    if (!m_net) return;

    QMap<QString, QGraphicsItem*> items;

    int x = -200;
    for (auto &place : m_net->places()) {
        auto *item = new PlaceItem(place);
        item->setPos(x, -80);
        m_scene->addItem(item);
        items[place->id()] = item;
        x += 120;
    }

    x = -140;
    for (auto &t : m_net->transitions()) {
        auto *item = new TransitionItem(t);
        item->setPos(x, 80);
        m_scene->addItem(item);
        items[t->id()] = item;
        x += 120;
    }

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