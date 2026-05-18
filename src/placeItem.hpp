/**
 * @file placeItem.hpp
 * @authors xdurecs00, xpertod00
 * @brief Graficka reprezentacia miesta (kruh) v Petriho sieti.
 */

#pragma once
#include <QGraphicsEllipseItem>
#include <memory>
#include "place.hpp"

class ArcItem;

/**
 * @brief Graficka reprezentacia miesta — kresli sa ako kruh.
 * Farba kruhu sa meni podla poctu tokenov. Dvojklik otvori dialog pre editaciu.
 */
class PlaceItem : public QGraphicsEllipseItem {
public:
    static constexpr qreal R = 30.0; ///< Polomer kruhu

    /**
     * @brief Konstruktor.
     * @param place Pointer na model miesta.
     * @param parent Rodicovsky prvok.
     */
    explicit PlaceItem(std::shared_ptr<Place> place,
                       QGraphicsItem *parent = nullptr);

    /** @brief Prekrelsli kruh po zmene tokenov. */
    void refresh();
    /** @brief Prida hranu pre aktualizaciu pri pohybe. */
    void addArc(ArcItem *arc);
    /** @brief Vrati pointer na model miesta. */
    std::shared_ptr<Place> place() const { return m_place; }

protected:
    /** @brief Vykresli kruh s nazvom a tokenmi. */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    /** @brief Aktualizuje hrany pri pohybe miesta. */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    /** @brief Otvori dialog pre editaciu miesta. */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    std::shared_ptr<Place> m_place; ///< Model miesta
    QVector<ArcItem*>      m_arcs;  ///< Hrany napojene na toto miesto
};