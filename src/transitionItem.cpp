// Autori: xdurec00, xpertod00
// graficka reprezentacia prechodu

#include "transitionItem.hpp"
#include "arcItem.hpp"
#include <QPainter>

TransitionItem::TransitionItem(std::shared_ptr<Transition> transition,
                               QGraphicsItem *parent)
    : QGraphicsRectItem(-W/2, -H/2, W, H, parent)
    , m_transition(transition)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::black, 2));
}

void TransitionItem::refresh() {
    update();
}

void TransitionItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawRect(boundingRect());

    painter->setFont(QFont("Arial", 9, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, m_transition->id());
}

void TransitionItem::addArc(ArcItem *arc)
{
    m_arcs.append(arc);
}

QVariant TransitionItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
    {
        for(ArcItem *arc : m_arcs)
        {
            arc->updateGeometry();
        }
    }

    return QGraphicsRectItem::itemChange(change, value);
}