// Autori: xdurec00
// graficka reprezentacia hrany

#include "arcItem.hpp"
#include <QPainter>
#include <QtMath>

ArcItem::ArcItem(QGraphicsItem *from, QGraphicsItem *to,
                 int weight, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
    , m_from(from)
    , m_to(to)
    , m_weight(weight)
{
    setZValue(-1); // kreslí sa pod miestami a prechodmi
    setPen(QPen(Qt::white, 1.5));
    updateGeometry();
}

void ArcItem::updateGeometry()
{
    QPointF src = m_from->mapToScene(m_from->boundingRect().center());
    QPointF dst = m_to->mapToScene(m_to->boundingRect().center());
    setLine(QLineF(src, dst));
}

QRectF ArcItem::boundingRect() const
{
    return QGraphicsLineItem::boundingRect().adjusted(-20, -20, 20, 20);
}

void ArcItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)

    QLineF l = line();
    if (l.length() < 1.0) return;

    painter->setPen(QPen(Qt::white, 1.5));
    painter->drawLine(l);

    // sipka na konci
    const double angle = std::atan2(-l.dy(), l.dx());
    const double sz = 10.0;
    QPointF tip = l.p2();
    QPointF p1 = tip + QPointF(std::cos(angle + M_PI*5/6) * sz,
                               -std::sin(angle + M_PI*5/6) * sz);
    QPointF p2 = tip + QPointF(std::cos(angle - M_PI*5/6) * sz,
                               -std::sin(angle - M_PI*5/6) * sz);
    painter->setBrush(Qt::white);
    painter->drawPolygon(QPolygonF({tip, p1, p2}));

    if (m_weight > 1) {
        QPointF mid = (l.p1() + l.p2()) / 2.0;
        painter->setPen(Qt::yellow);
        painter->setFont(QFont("Arial", 8));
        painter->drawText(mid + QPointF(4, -4), QString::number(m_weight));
    }
}