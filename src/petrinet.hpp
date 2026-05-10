// Autori: xdurecs00, x
// petrinet

#pragma once
#include "place.hpp"
#include "transition.hpp"
#include <QString>
#include <QVector>
#include <memory>

class PetriNet {
public:
    // metadata
    QString name()    const { return m_name; }
    QString comment() const { return m_comment; }
    void setName   (const QString &n) { m_name = n; }
    void setComment(const QString &c) { m_comment = c; }

    // vstupy/vystupy
    void addInput (const QString &s) { m_inputs.append(s); }
    void addOutput(const QString &s) { m_outputs.append(s); }
    const QVector<QString> &inputs()  const { return m_inputs; }
    const QVector<QString> &outputs() const { return m_outputs; }

    // premenne
    void addVariable(const QString &decl) { m_variables.append(decl); }
    const QVector<QString> &variables() const { return m_variables; }

    // miesta
    std::shared_ptr<Place> addPlace(const QString &id, int tokens = 0);
    std::shared_ptr<Place> findPlace(const QString &id) const;
    void removePlace(const QString &id);
    const QVector<std::shared_ptr<Place>> &places() const { return m_places; }

    // prechody
    std::shared_ptr<Transition> addTransition(const QString &id);
    std::shared_ptr<Transition> findTransition(const QString &id) const;
    const QVector<std::shared_ptr<Transition>> &transitions() const { return m_transitions; }

    // runtime
    void resetMarking();
    bool isEnabled(const std::shared_ptr<Transition> &t) const;

private:
    QString m_name;
    QString m_comment;
    QVector<QString> m_inputs;
    QVector<QString> m_outputs;
    QVector<QString> m_variables;
    QVector<std::shared_ptr<Place>>      m_places;
    QVector<std::shared_ptr<Transition>> m_transitions;
};