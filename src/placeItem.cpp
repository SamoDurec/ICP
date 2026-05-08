// Autori: xdurec00, xpertod00
// graficka reprezentacia miesta

#include "placeItem.hpp"
#include "arcItem.hpp"
#include <QPainter>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>

PlaceItem::PlaceItem(std::shared_ptr<Place> place, QGraphicsItem *parent)
    : QGraphicsEllipseItem(-R, -R, 2*R, 2*R, parent)
    , m_place(place)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::black, 2));
}

void PlaceItem::refresh() {
    update();
}

void PlaceItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)

    // farba podla tokenov
    QColor bg = (m_place->tokens() > 0) ? QColor(180, 230, 180) : Qt::white;
    painter->setBrush(bg);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(boundingRect());

    // meno miesta
    painter->setFont(QFont("Arial", 9, QFont::Bold));
    painter->drawText(boundingRect().adjusted(0, -10, 0, -10),
                      Qt::AlignCenter, m_place->id());

    // pocet tokenov
    if (m_place->tokens() > 0) {
        painter->setFont(QFont("Arial", 11, QFont::Bold));
        painter->drawText(boundingRect().adjusted(0, 6, 0, 6),
                          Qt::AlignCenter,
                          QString::number(m_place->tokens()));
    }
}

void PlaceItem::addArc(ArcItem *arc)
{
    m_arcs.append(arc);
}

QVariant PlaceItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
    {
        for(ArcItem *arc : m_arcs)
        {
            arc->updateGeometry();
        }
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

void PlaceItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    bool ok;

    QString newId = QInputDialog::getText(
        nullptr,
        "Edit place",
        "Place name: ",
        QLineEdit::Normal,
        m_place->id(),
        &ok);
    
    if (ok && !newId.isEmpty())
    {
        m_place->setId(newId);
        update();
    }
}