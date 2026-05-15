// // Autori: xdurecs00, xpertod00
// // transition v Petriho sieti

// #pragma once
// #include <QString>
// #include <QVector>
// #include <QPointF>

// // Hrana s vahou: id miesta + pocet tokenov
// struct Arc {
//     QString placeId;
//     int     weight;
// };

// class Transition {
// public:
//     Transition(const QString &id);

//     QString         id()         const { return m_id; }
//     QString         eventName()  const { return m_eventName; }
//     QString         guard()      const { return m_guard; }
//     int             delayMs()    const { return m_delayMs; }
//     QString         action()     const { return m_action; }
//     QVector<Arc>    inputArcs()  const { return m_inputArcs; }
//     QVector<Arc>    outputArcs() const { return m_outputArcs; }
//     bool            isDelayed()  const { return m_delayMs >= 0; }

//     void setEventName(const QString &e) { m_eventName = e; }
//     void setGuard    (const QString &g) { m_guard = g; }
//     void setDelayMs  (int d)            { m_delayMs = d; }
//     void setAction   (const QString &a) { m_action = a; }
//     void addInputArc (const Arc &a)     { m_inputArcs.append(a); }
//     void addOutputArc(const Arc &a)     { m_outputArcs.append(a); }
//     void setInputArcs(const QVector<Arc> &arcs) { m_inputArcs = arcs; }
//     void setOutputArcs(const QVector<Arc> &arcs) { m_outputArcs = arcs; }

//     QPointF pos() const {return m_pos; }
//     void setPos(const QPointF &p)
//     {
//         m_pos = p;
//     }

//     void setId(const QString &id);

// private:
//     QString      m_id;
//     QString      m_eventName;
//     QString      m_guard;
//     int          m_delayMs  {-1};
//     QString      m_action;
//     QVector<Arc> m_inputArcs;
//     QVector<Arc> m_outputArcs;
//     QPointF      m_pos {0, 0};
// };





/**
 * @file transition.hpp
 * @authors xdurecs00, xpertod00
 * @brief Reprezentacia transition v Petriho sieti.
 */

#pragma once
#include <QString>
#include <QVector>
#include <QPointF>

/**
 * @brief Hrana s vahou spajajuca place a transition.
 */
struct Arc {
    QString placeId; ///< Identifikator miesta
    int     weight;  ///< Vaha hrany
};

/**
 * @brief Prechod v Petriho sieti.
 *
 * Obsahuje podmienku odpálenia (event, guard, delay) a akciu.
 */
class Transition {
public:
    /** @brief Konstruktor. @param id Identifikator prechodu. */
    Transition(const QString &id);

    /** @brief Vrati identifikator. */
    QString      id()         const { return m_id; }
    /** @brief Vrati nazov vstupnej udalosti. */
    QString      eventName()  const { return m_eventName; }
    /** @brief Vrati straznu podmienku. */
    QString      guard()      const { return m_guard; }
    /** @brief Vrati oneskorenie v ms (-1 = ziadne). */
    int          delayMs()    const { return m_delayMs; }
    /** @brief Vrati akciu prechodu. */
    QString      action()     const { return m_action; }
    /** @brief Vrati vstupne hrany. */
    QVector<Arc> inputArcs()  const { return m_inputArcs; }
    /** @brief Vrati vystupne hrany. */
    QVector<Arc> outputArcs() const { return m_outputArcs; }
    /** @brief Vrati true ak ma prechod oneskorenie. */
    bool         isDelayed()  const { return m_delayMs >= 0; }

    /** @brief Nastavi nazov udalosti. */
    void setEventName (const QString &e)       { m_eventName = e; }
    /** @brief Nastavi straznu podmienku. */
    void setGuard     (const QString &g)       { m_guard = g; }
    /** @brief Nastavi oneskorenie. */
    void setDelayMs   (int d)                  { m_delayMs = d; }
    /** @brief Nastavi akciu. */
    void setAction    (const QString &a)       { m_action = a; }
    /** @brief Prida vstupnu hranu. */
    void addInputArc  (const Arc &a)           { m_inputArcs.append(a); }
    /** @brief Prida vystupnu hranu. */
    void addOutputArc (const Arc &a)           { m_outputArcs.append(a); }
    /** @brief Nastavi vstupne hrany. */
    void setInputArcs (const QVector<Arc> &arcs) { m_inputArcs = arcs; }
    /** @brief Nastavi vystupne hrany. */
    void setOutputArcs(const QVector<Arc> &arcs) { m_outputArcs = arcs; }
    /** @brief Vrati poziciu v GUI. */
    QPointF pos() const { return m_pos; }
    /** @brief Nastavi poziciu v GUI. */
    void setPos(const QPointF &p) { m_pos = p; }
    /** @brief Nastavi identifikator. */
    void setId(const QString &id);

private:
    QString      m_id;                ///< Identifikator
    QString      m_eventName;         ///< Nazov vstupnej udalosti
    QString      m_guard;             ///< Strazna podmienka
    int          m_delayMs  {-1};     ///< Oneskorenie v ms
    QString      m_action;            ///< Akcia prechodu
    QVector<Arc> m_inputArcs;         ///< Vstupne hrany
    QVector<Arc> m_outputArcs;        ///< Vystupne hrany
    QPointF      m_pos {0, 0};        ///< Pozicia v GUI
};