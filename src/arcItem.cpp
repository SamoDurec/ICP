/**
 * @file arcItem.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie hrany.
 */

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
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(-1); // kreslí sa pod miestami a prechodmi
    setPen(QPen(Qt::white, 1.5));
    updateGeometry();
}

void ArcItem::updateGeometry()
{
    // najde stred objektu
    QPointF srcCenter = m_from->mapToScene(m_from->boundingRect().center());
    QPointF dstCenter = m_to->mapToScene(m_to->boundingRect().center());

    // spojnice stredu
    QLineF centerLine(srcCenter, dstCenter);

    // nevykresluj, pokud je hrana prilis kratka
    if (centerLine.length() < 1.0) return;

    // posunuti koncu car
    QPointF src = edgePoint(m_from, dstCenter);
    QPointF dst = edgePoint(m_to, srcCenter);

    setLine(QLineF(src, dst));
}

QPointF ArcItem::edgePoint(QGraphicsItem *item, const QPointF &to)
{
    // slouzi k nalezeni okraju mist a přechodu, aby se spravne vykreslovaly hrany
    // hleda okraj pro obdelnik, coz by melo fungovat i pro kruh
    QRectF rect = item->sceneBoundingRect();

    QPointF center = rect.center();

    QLineF line(center, to);

    // nastaveni hran
    QList<QLineF> edges = {
        QLineF(rect.topLeft(), rect.topRight()),
        QLineF(rect.topRight(), rect.bottomRight()),
        QLineF(rect.bottomRight(), rect.bottomLeft()),
        QLineF(rect.bottomLeft(), rect.topLeft())
    };

    QPointF intersection;

    for (const QLineF &edge : edges)
    {
        auto type = line.intersects(edge, &intersection);

        if (type == QLineF::BoundedIntersection) return intersection;
    }

    return center;
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

    if (isSelected()) painter->setPen(QPen(Qt::gray, 1.5));
    else painter->setPen(QPen(Qt::black, 1.5));
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