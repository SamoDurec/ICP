/**
 * @file arcItem.hpp
 * @authors xdurecs00, xpertod00
 * @brief Graficka reprezentacia hrany (sipka) v Petriho sieti.
 */

#pragma once
#include <QGraphicsLineItem>

/**
 * @brief Graficka hrana medzi miestom a prechodom.
 * Kreslí sa ako ciara so sipkou na konci. Aktualizuje sa pri pohybe prvkov.
 */
class ArcItem : public QGraphicsLineItem {
public:
    /**
     * @brief Konstruktor.
     * @param from Zdrojovy prvok.
     * @param to Cielovy prvok.
     * @param weight Vaha hrany.
     * @param parent Rodicovsky prvok.
     */
    ArcItem(QGraphicsItem *from, QGraphicsItem *to,
            int weight = 1, QGraphicsItem *parent = nullptr);

    /** @brief Prepocita polohu sipky podla aktualnych pozicii prvkov. */
    void updateGeometry();

    /** @brief Vrati zdrojovy prvok. */
    QGraphicsItem* fromItem() const { return m_from; }
    /** @brief Vrati cielovy prvok. */
    QGraphicsItem* toItem()   const { return m_to; }
    /** @brief Vrati vahu hrany. */
    int weight()              const { return m_weight; }

protected:
    /** @brief Vykresli hranu so sipkou. */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    /** @brief Vrati ohranicujuci obdlznik. */
    QRectF boundingRect() const override;

private:
    QGraphicsItem *m_from;   ///< Zdrojovy prvok
    QGraphicsItem *m_to;     ///< Cielovy prvok
    int            m_weight; ///< Vaha hrany
    /** @brief Najde bod na okraji prvku smerom k cielovemu bodu. */
    QPointF edgePoint(QGraphicsItem *item, const QPointF &to);
};