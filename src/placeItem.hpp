// Autori: xdurec00
// graficka reprezentacia miesta

#pragma once
#include <QGraphicsEllipseItem>
#include <memory>
#include "place.hpp"

class PlaceItem : public QGraphicsEllipseItem {
public:
    static constexpr qreal R = 30.0;

    explicit PlaceItem(std::shared_ptr<Place> place,
                       QGraphicsItem *parent = nullptr);

    void refresh(); // prekreslenie po zmene tokenov

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    std::shared_ptr<Place> m_place;
};