// Autori: xdurec00
// graficka reprezentacia prechodu

#include "transitionItem.hpp"
#include <QPainter>

TransitionItem::TransitionItem(std::shared_ptr<Transition> transition,
                               QGraphicsItem *parent)
    : QGraphicsRectItem(-W/2, -H/2, W, H, parent)
    , m_transition(transition)
{
    setFlag(QGraphicsItem::ItemIsMovable);
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