// Autori: xdurecs00, xpertod00
// graficka reprezentacia hrany

#pragma once
#include <QGraphicsLineItem>

class ArcItem : public QGraphicsLineItem {
public:
    ArcItem(QGraphicsItem *from, QGraphicsItem *to,
            int weight = 1, QGraphicsItem *parent = nullptr);

    void updateGeometry(); // prepocita polohu sipky

    QGraphicsItem* fromItem() const
    {
        return m_from;
    }

    QGraphicsItem* toItem() const
    {
        return m_to;
    }

    int weight() const
    {
        return m_weight;
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;

private:
    QGraphicsItem *m_from;
    QGraphicsItem *m_to;
    int            m_weight;
    QPointF edgePoint(QGraphicsItem *item, const QPointF &to);
};