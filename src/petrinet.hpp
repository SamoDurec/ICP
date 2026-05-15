// // Autori: xdurecs00, xpertod00
// // petrinet

// #pragma once
// #include "place.hpp"
// #include "transition.hpp"
// #include <QString>
// #include <QVector>
// #include <memory>

// class PetriNet {
// public:
//     // metadata
//     QString name()    const { return m_name; }
//     QString comment() const { return m_comment; }
//     void setName   (const QString &n) { m_name = n; }
//     void setComment(const QString &c) { m_comment = c; }

//     // vstupy/vystupy
//     void addInput (const QString &s) { m_inputs.append(s); }
//     void addOutput(const QString &s) { m_outputs.append(s); }
//     const QVector<QString> &inputs()  const { return m_inputs; }
//     const QVector<QString> &outputs() const { return m_outputs; }

//     // premenne
//     void addVariable(const QString &decl) { m_variables.append(decl); }
//     const QVector<QString> &variables() const { return m_variables; }

//     // miesta
//     std::shared_ptr<Place> addPlace(const QString &id, int tokens = 0);
//     std::shared_ptr<Place> findPlace(const QString &id) const;
//     void removePlace(const QString &id);
//     const QVector<std::shared_ptr<Place>> &places() const { return m_places; }

//     // prechody
//     std::shared_ptr<Transition> addTransition(const QString &id);
//     std::shared_ptr<Transition> findTransition(const QString &id) const;
//     void removeTransition(const QString &id);
//     const QVector<std::shared_ptr<Transition>> &transitions() const { return m_transitions; }

//     // runtime
//     void resetMarking();
//     bool isEnabled(const std::shared_ptr<Transition> &t) const;

// private:
//     QString m_name;
//     QString m_comment;
//     QVector<QString> m_inputs;
//     QVector<QString> m_outputs;
//     QVector<QString> m_variables;
//     QVector<std::shared_ptr<Place>>      m_places;
//     QVector<std::shared_ptr<Transition>> m_transitions;
// };






/**
 * @file petrinet.hpp
 * @authors xdurecs00, xpertod00
 * @brief Hlavna trieda reprezentujuca Petriho siet.
 */

#pragma once
#include "place.hpp"
#include "transition.hpp"
#include <QString>
#include <QVector>
#include <memory>

/**
 * @brief Petriho siet — obsahuje miesta, prechody, vstupy, vystupy a premenne.
 */
class PetriNet {
public:
    /** @brief Vrati nazov siete. */
    QString name()    const { return m_name; }
    /** @brief Vrati komentar siete. */
    QString comment() const { return m_comment; }
    /** @brief Nastavi nazov siete. */
    void setName   (const QString &n) { m_name = n; }
    /** @brief Nastavi komentar siete. */
    void setComment(const QString &c) { m_comment = c; }

    /** @brief Prida vstup. */
    void addInput (const QString &s) { m_inputs.append(s); }
    /** @brief Prida vystup. */
    void addOutput(const QString &s) { m_outputs.append(s); }
    /** @brief Vrati zoznam vstupov. */
    const QVector<QString> &inputs()  const { return m_inputs; }
    /** @brief Vrati zoznam vystupov. */
    const QVector<QString> &outputs() const { return m_outputs; }

    /** @brief Prida premennu. */
    void addVariable(const QString &decl) { m_variables.append(decl); }
    /** @brief Vrati zoznam premennych. */
    const QVector<QString> &variables() const { return m_variables; }

    /** @brief Prida miesto. @return Pointer na nove miesto. */
    std::shared_ptr<Place> addPlace(const QString &id, int tokens = 0);
    /** @brief Najde miesto podla id. @return Pointer alebo nullptr. */
    std::shared_ptr<Place> findPlace(const QString &id) const;
    /** @brief Odstrani miesto a jeho hrany. */
    void removePlace(const QString &id);
    /** @brief Vrati vsetky miesta. */
    const QVector<std::shared_ptr<Place>> &places() const { return m_places; }

    /** @brief Prida prechod. @return Pointer na novy prechod. */
    std::shared_ptr<Transition> addTransition(const QString &id);
    /** @brief Najde prechod podla id. @return Pointer alebo nullptr. */
    std::shared_ptr<Transition> findTransition(const QString &id) const;
    /** @brief Odstrani prechod. */
    void removeTransition(const QString &id);
    /** @brief Vrati vsetky prechody. */
    const QVector<std::shared_ptr<Transition>> &transitions() const { return m_transitions; }

    /** @brief Resetuje marking na pociatocne hodnoty. */
    void resetMarking();
    /** @brief Skontroluje ci je prechod povoleny (dostatok tokenov). */
    bool isEnabled(const std::shared_ptr<Transition> &t) const;

private:
    QString m_name;                                      ///< Nazov siete
    QString m_comment;                                   ///< Komentar
    QVector<QString> m_inputs;                           ///< Vstupy
    QVector<QString> m_outputs;                          ///< Vystupy
    QVector<QString> m_variables;                        ///< Premenne
    QVector<std::shared_ptr<Place>>      m_places;       ///< Miesta
    QVector<std::shared_ptr<Transition>> m_transitions;  ///< Prechody
};