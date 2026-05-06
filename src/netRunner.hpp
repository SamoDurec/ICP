// Autori: xdurec00
// Runtime - spustanie Petriho siete

#pragma once
#include <QObject>
#include <QTimer>
#include <QHash>
#include <memory>
#include "petrinet.hpp"

class NetRunner : public QObject {
    Q_OBJECT
public:
    explicit NetRunner(std::shared_ptr<PetriNet> net,
                       QObject *parent = nullptr);

    void start();
    void stop();
    bool isRunning() const { return m_running; }

    // Injektovanie vstupu zvonku
    void injectInput(const QString &name, const QString &value);

signals:
    void logMessage(const QString &msg);
    void markingChanged();

private slots:
    void onTimer(const QString &transitionId);

private:
    void stabilise();
    bool tryFire(std::shared_ptr<Transition> t);
    bool isEnabled(std::shared_ptr<Transition> t) const;
    void scheduleDelayed();
    void log(const QString &msg);

    std::shared_ptr<PetriNet>    m_net;
    bool                         m_running {false};
    QHash<QString, QString>      m_inputValues;
    QHash<QString, QTimer*>      m_timers;
    QString                      m_lastEvent;
};