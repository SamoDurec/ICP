// Autori: xdurec00
// graficka reprezentacia hrany

#pragma once
#include <QGraphicsLineItem>

class ArcItem : public QGraphicsLineItem {
public:
    ArcItem(QGraphicsItem *from, QGraphicsItem *to,
            int weight = 1, QGraphicsItem *parent = nullptr);

    void updateGeometry(); // prepocita polohu sipky

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QRectF boundingRect() const override;

private:
    QGraphicsItem *m_from;
    QGraphicsItem *m_to;
    int            m_weight;
};