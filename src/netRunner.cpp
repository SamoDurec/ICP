// Autori: xdurec00
// Implementacia NetRunner

#include "netRunner.hpp"
#include <QDateTime>
#include <QDebug>

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
    // Token check
    if (!m_net->isEnabled(t)) return false;

    // Event check
    if (!t->eventName().isEmpty() && t->eventName() != m_lastEvent)
        return false;

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