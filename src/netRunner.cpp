/**
 * @file netRunner.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie spustenia siete.
 */

#include "netRunner.hpp"
#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>

NetRunner::NetRunner(std::shared_ptr<PetriNet> net, QObject *parent)
    : QObject(parent)
    , m_net(net)
{}

void NetRunner::start()
{
    if (m_running) return;
    m_running = true;
    m_net->resetMarking();
    log("[START] " + m_net->name());
    emit markingChanged();
    stabilise();
}

void NetRunner::stop()
{
    if (!m_running) return;
    m_running = false;
    for (auto *t : m_timers) { t->stop(); t->deleteLater(); }
    m_timers.clear();
    log("[STOP]");
    emit markingChanged();
}

void NetRunner::injectInput(const QString &name, const QString &value)
{
    m_inputValues[name] = value;
    m_lastEvent = name;
    log("[INPUT] " + name + " = " + value);
    stabilise();
    scheduleDelayed();
}

void NetRunner::onTimer(const QString &transitionId)
{
    m_timers.remove(transitionId);
    if (!m_running) return;
    auto t = m_net->findTransition(transitionId);
    if (!t) return;
    log("[TIMEOUT] " + transitionId);
    if (isEnabled(t)) {
        tryFire(t);
        emit markingChanged();
        stabilise();
    } else {
        log("[TIMEOUT IGNORED] " + transitionId);
    }
}

void NetRunner::stabilise()
{
    bool anyFired = true;
    while (anyFired) {
        anyFired = false;
        for (auto &t : m_net->transitions()) {
            if (!t->isDelayed() && isEnabled(t)) {
                if (tryFire(t)) anyFired = true;
            }
        }
    }
    emit markingChanged();
}

bool NetRunner::isEnabled(std::shared_ptr<Transition> t) const
{
    if (!m_net->isEnabled(t)) return false;

    if (!t->eventName().isEmpty() && t->eventName() != m_lastEvent)
        return false;

    // vyhodnotenie guardu
    if (!t->guard().isEmpty() && !evaluateGuard(t->guard())) {
        return false;
    }

    return true;
}

bool NetRunner::tryFire(std::shared_ptr<Transition> t)
{
    if (!isEnabled(t)) return false;

    // odobere tokeny
    for (const auto &arc : t->inputArcs()) {
        auto p = m_net->findPlace(arc.placeId);
        if (p) p->removeTokens(arc.weight);
    }

    log("[FIRE] " + t->id());

    // prida tokeny
    for (const auto &arc : t->outputArcs()) {
        auto p = m_net->findPlace(arc.placeId);
        if (p) p->addTokens(arc.weight);
    }

    m_lastEvent.clear();
    return true;
}

void NetRunner::scheduleDelayed()
{
    for (auto &t : m_net->transitions()) {
        if (!t->isDelayed()) continue;
        if (m_timers.contains(t->id())) continue;
        if (!m_net->isEnabled(t)) continue;

        int delay = t->delayMs();
        auto *timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(delay);
        const QString tid = t->id();
        connect(timer, &QTimer::timeout, this, [this, tid]{ onTimer(tid); });
        timer->start();
        m_timers.insert(t->id(), timer);
        log("[TIMER] " + t->id() + " za " + QString::number(delay) + "ms");
    }
}

void NetRunner::log(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    emit logMessage(ts + "  " + msg);
}

QString NetRunner::valueof(const QString &name) const
{
    return m_inputValues.value(name, "");
}

bool NetRunner::evaluateGuard(const QString &guard) const
{
    if (guard.isEmpty()) {
        return true;
    }

    QString g = guard.trimmed();

    // nahrad valueof("x") skutocnou hodnotou
    QRegularExpression valRe("valueof\\(\"([^\"]+)\"\\)");
    QRegularExpressionMatchIterator it = valRe.globalMatch(g);
    while (it.hasNext()) {
        QRegularExpressionMatch m = it.next();
        QString inputName = m.captured(1);
        QString val = valueof(inputName);
        g.replace(m.captured(0), "\"" + val + "\"");
    }

    // defined("x") -> true ak bol vstup nastaveny
    QRegularExpression defRe("defined\\(\"([^\"]+)\"\\)");
    QRegularExpressionMatchIterator it3 = defRe.globalMatch(g);
    while (it3.hasNext()) {
        QRegularExpressionMatch md = it3.next();
        QString inputName = md.captured(1);
        bool isDefined = m_inputValues.contains(inputName);
        g.replace(md.captured(0), isDefined ? "1" : "0");
    }

    // atoi("123") -> 123
    QRegularExpression atoiRe("atoi\\(\"(-?\\d+)\"\\)");
    QRegularExpressionMatch m2;
    while ((m2 = atoiRe.match(g)).hasMatch()) {
        g.replace(m2.captured(0), m2.captured(1));
    }

    // tokens("place") -> pocet tokenov v mieste
    QRegularExpression tokRe("tokens\\(\"([^\"]+)\"\\)");
    QRegularExpressionMatchIterator it2 = tokRe.globalMatch(g);
    while (it2.hasNext()) {
        QRegularExpressionMatch mt = it2.next();
        QString placeName = mt.captured(1);
        auto place = m_net->findPlace(placeName);
        int cnt = place ? place->tokens() : 0;
        g.replace(mt.captured(0), QString::number(cnt));
    }

    // vyhodnotenie: "123" == 1, "abc" != "xyz" atd.
    // podporujeme: ==, !=, >=, <=, >, 
    QRegularExpression cmpRe("(-?\\d+)\\s*(==|!=|>=|<=|>|<)\\s*(-?\\d+)");
    QRegularExpressionMatch mc = cmpRe.match(g);
    if (mc.hasMatch()) {
        int left  = mc.captured(1).toInt();
        QString op = mc.captured(2);
        int right = mc.captured(3).toInt();

        if (op == "==") return left == right;
        if (op == "!=") return left != right;
        if (op == ">=") return left >= right;
        if (op == "<=") return left <= right;
        if (op == ">")  return left > right;
        if (op == "<")  return left < right;
    }

    // string porovnanie: "abc" == "abc"
    QRegularExpression strRe("\"([^\"]*)\"\\s*(==|!=)\\s*\"([^\"]*)\"");
    QRegularExpressionMatch ms = strRe.match(g);
    if (ms.hasMatch()) {
        QString left  = ms.captured(1);
        QString op    = ms.captured(2);
        QString right = ms.captured(3);
        if (op == "==") return left == right;
        if (op == "!=") return left != right;
    }

    // ak nevieme vyhodnotit, prepustime
    return true;
}