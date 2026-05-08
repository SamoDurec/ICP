// Autori: xdurec00, xpertod00
// graficka reprezentacia prechodu

#pragma once
#include <QGraphicsRectItem>
#include <memory>
#include "transition.hpp"

class ArcItem;

class TransitionItem : public QGraphicsRectItem {
public:
    static constexpr qreal W = 60.0;
    static constexpr qreal H = 30.0;

    explicit TransitionItem(std::shared_ptr<Transition> transition,
                            QGraphicsItem *parent = nullptr);

    void refresh();

    void addArc(ArcItem *arc);

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    std::shared_ptr<Transition> m_transition;
    QVector<ArcItem*> m_arcs;
};