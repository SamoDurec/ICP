/**
 * @file netRunner.hpp
 * @authors xdurecs00, xpertod00
 * @brief Event-driven interpreter Petriho siete.
 */

#pragma once
#include <QObject>
#include <QTimer>
#include <QHash>
#include <memory>
#include "petrinet.hpp"

/**
 * @brief Spusta interpretovanu Petriho siet.
 *
 * Implementuje hlavnu slucku: stabilizacia, timery, inject vstupov.
 */
class NetRunner : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param net Siet na spustenie.
     * @param parent Rodicovsky objekt.
     */
    explicit NetRunner(std::shared_ptr<PetriNet> net,
                       QObject *parent = nullptr);

    /** @brief Spusti siet (reset markingu, stabilizacia). */
    void start();
    /** @brief Zastavi siet a zrusi timery. */
    void stop();
    /** @brief Vrati true ak siet bezi. */
    bool isRunning() const { return m_running; }
    /** @brief Vrati true ak ma prechod aktivny timer. */
    bool isPendingTimer(const QString &id) const { return m_timers.contains(id); }
    /**
     * @brief Injektuje vstupnu udalost do siete.
     * @param name Nazov vstupu.
     * @param value Hodnota vstupu.
     */
    void injectInput(const QString &name, const QString &value);

signals:
    /** @brief Signal pre logovanie sprav. */
    void logMessage(const QString &msg);
    /** @brief Signal po zmene markingu. */
    void markingChanged();

private slots:
    /** @brief Vola sa po vyprsani timera prechodu. */
    void onTimer(const QString &transitionId);

private:
    /** @brief Stabilizacna slucka — odpaluje okamzite prechody. */
    void stabilise();
    /** @brief Pokusi sa odpálit prechod. @return true ak uspesne. */
    bool tryFire(std::shared_ptr<Transition> t);
    /** @brief Skontroluje ci je prechod povoleny. */
    bool isEnabled(std::shared_ptr<Transition> t) const;
    /** @brief Naplánuje timery pre oneskorene prechody. */
    void scheduleDelayed();
    /** @brief Zapise spravu do logu. */
    void log(const QString &msg);
    /** @brief Vyhodnotí strážnu podmienku prechodu. @return true ak podmienka platí. */
    bool evaluateGuard(const QString &guard) const;
    /** @brief Vráti poslednú známu hodnotu vstupu. */
    QString valueof(const QString &name) const;

    std::shared_ptr<PetriNet>    m_net;           ///< Spustana siet
    bool                         m_running {false}; ///< Stav behu
    QHash<QString, QString>      m_inputValues;   ///< Posledne zname hodnoty vstupov
    QHash<QString, QTimer*>      m_timers;         ///< Aktivne timery
    QString                      m_lastEvent;      ///< Posledna vstupna udalost
};