// Autori: xdurecs00, x
// transition v Petriho sieti

#pragma once
#include <QString>
#include <QVector>
#include <QPointF>

// Hrana s vahou: id miesta + pocet tokenov
struct Arc {
    QString placeId;
    int     weight;
};

class Transition {
public:
    Transition(const QString &id);

    QString         id()         const { return m_id; }
    QString         eventName()  const { return m_eventName; }
    QString         guard()      const { return m_guard; }
    int             delayMs()    const { return m_delayMs; }
    QString         action()     const { return m_action; }
    QVector<Arc>    inputArcs()  const { return m_inputArcs; }
    QVector<Arc>    outputArcs() const { return m_outputArcs; }
    bool            isDelayed()  const { return m_delayMs >= 0; }

    void setEventName(const QString &e) { m_eventName = e; }
    void setGuard    (const QString &g) { m_guard = g; }
    void setDelayMs  (int d)            { m_delayMs = d; }
    void setAction   (const QString &a) { m_action = a; }
    void addInputArc (const Arc &a)     { m_inputArcs.append(a); }
    void addOutputArc(const Arc &a)     { m_outputArcs.append(a); }
    void setInputArcs(const QVector<Arc> &arcs) { m_inputArcs = arcs; }
    void setOutputArcs(const QVector<Arc> &arcs) { m_outputArcs = arcs; }

    QPointF pos() const {return m_pos; }
    void setPos(const QPointF &p)
    {
        m_pos = p;
    }

    void setId(const QString &id);

private:
    QString      m_id;
    QString      m_eventName;
    QString      m_guard;
    int          m_delayMs  {-1};
    QString      m_action;
    QVector<Arc> m_inputArcs;
    QVector<Arc> m_outputArcs;
    QPointF      m_pos;
};