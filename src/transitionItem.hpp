/**
 * @file transitionItem.hpp
 * @authors xdurecs00, xpertod00
 * @brief Graficka reprezentacia prechodu (obdlznik) v Petriho sieti.
 */

#pragma once
#include <QGraphicsRectItem>
#include <memory>
#include "transition.hpp"

class ArcItem;

/**
 * @brief Graficka reprezentacia prechodu — kresli sa ako obdlznik.
 *
 * Farba sa meni podla stavu: biela=normal, zlta=enabled, modra=timer.
 * Dvojklik otvori dialog pre editaciu.
 */
class TransitionItem : public QGraphicsRectItem {
public:
    static constexpr qreal W = 60.0; ///< Sirka obdlznika
    static constexpr qreal H = 30.0; ///< Vyska obdlznika

    /** @brief Stav prechodu pre farebne zvyraznenie. */
    enum class State { Normal, Enabled, PendingTimer };

    /** @brief Nastavi stav a prekresli. */
    void setState(State s) { m_state = s; update(); }

    /**
     * @brief Konstruktor.
     * @param transition Pointer na model prechodu.
     * @param parent Rodicovsky prvok.
     */
    explicit TransitionItem(std::shared_ptr<Transition> transition,
                            QGraphicsItem *parent = nullptr);

    /** @brief Prekresli obdlznik. */
    void refresh();
    /** @brief Prida hranu pre aktualizaciu pri pohybe. */
    void addArc(ArcItem *arc);
    /** @brief Vrati pointer na model prechodu. */
    std::shared_ptr<Transition> transition() const { return m_transition; }

protected:
    /** @brief Vykresli obdlznik s nazvom a farbou podla stavu. */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    /** @brief Aktualizuje hrany pri pohybe prechodu. */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    /** @brief Otvori dialog pre editaciu prechodu. */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    std::shared_ptr<Transition> m_transition;    ///< Model prechodu
    QVector<ArcItem*>           m_arcs;          ///< Hrany napojene na prechod
    State                       m_state {State::Normal}; ///< Aktualny stav
};