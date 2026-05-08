// Autori: xdurec00, xpertod00
// graficka reprezentacia miesta

#pragma once
#include <QGraphicsEllipseItem>
#include <memory>
#include "place.hpp"

class ArcItem;

class PlaceItem : public QGraphicsEllipseItem {
public:
    static constexpr qreal R = 30.0;

    explicit PlaceItem(std::shared_ptr<Place> place,
                       QGraphicsItem *parent = nullptr);

    void refresh(); // prekreslenie po zmene tokenov

    void addArc(ArcItem *arc); // posuvanie hran pri posunu miest

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    
private:
    std::shared_ptr<Place> m_place;
    QVector<ArcItem*> m_arcs;
};