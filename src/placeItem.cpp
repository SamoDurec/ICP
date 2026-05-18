/**
 * @file placeItem.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie miesta.
 */

#include "placeItem.hpp"
#include "arcItem.hpp"
#include <QPainter>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>

/**
 * @brief Vytvorenie grafickej reprezentácie miesta.
 */
PlaceItem::PlaceItem(std::shared_ptr<Place> place, QGraphicsItem *parent)
    : QGraphicsEllipseItem(-R, -R, 2*R, 2*R, parent)
    , m_place(place)
{
    // nastavenie flags
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::black, 2));
}

/**
 * @brief Prekreslenie miesta pri zmene.
 */
void PlaceItem::refresh() {
    update();
}

/**
 * @brief Hlavna vykreslovacia metoda.
 * 
 * Vykresli obrys, nazov a pocet tokenov (ak je vacsi ako 1).
 * Vybrane miesto je zafarbene sivo.
 * Sfarbenie miesta sa meni podla poctu tokenov.
 */
void PlaceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option) Q_UNUSED(widget)

    // farba podla tokenov
    QColor bg = (m_place->tokens() > 0) ? QColor(180, 230, 180) : Qt::white;
    painter->setBrush(bg);
    painter->setPen(QPen(Qt::black, 2));

    // obrys na zakliknutom mieste
    if (isSelected()) {
        painter->setPen(QPen(Qt::gray, 3));
    }

    painter->drawEllipse(boundingRect()); // vykreslenie obrysu

    // vykreslenie nazvu miesta
    painter->setFont(QFont("Arial", 9, QFont::Bold));
    painter->drawText(boundingRect().adjusted(0, -10, 0, -10), Qt::AlignCenter, m_place->id());

    // vykreslenie poctu tokenov
    if (m_place->tokens() > 0) {
        painter->setFont(QFont("Arial", 11, QFont::Bold));
        painter->drawText(boundingRect().adjusted(0, 6, 0, 6), Qt::AlignCenter, QString::number(m_place->tokens()));
    }
}

/**
 * @brief Zaistuje pridanie novych hran.
 */
void PlaceItem::addArc(ArcItem *arc) {
    m_arcs.append(arc);
}

/**
 * @brief Hlavna metoda zaistujuca vykreslovanie miest a hran pri zmene pozicie miesta.
 */
QVariant PlaceItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        m_place->setPos(value.toPointF());

        // najde vsetky pripojene hrany
        for(ArcItem *arc : m_arcs) {
            if (arc) {
                arc->updateGeometry(); // prekreslenie hran
            }
        }
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}


/**
 * @brief Zaistuje editaciu parametrov miesta.
 * Spustenie dvojklikom na miesto.
 */
void PlaceItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);

    bool ok;

    // editacia mena
    QString newId = QInputDialog::getText(
        nullptr,
        "Edit place",
        "Place name: ",
        QLineEdit::Normal,
        m_place->id(),
        &ok);

    if (!ok || newId.isEmpty()) {
        return;
    }

    // editacia tokenov
    int tokens = QInputDialog::getInt(
        nullptr,
        "Edit tokens",
        "Tokens: ",
        m_place->tokens(),
        0,
        999999,
        1,
        &ok);

    if (!ok) {
        return;
    }

    // aktualizacia parametrov
    m_place->setId(newId);
    m_place->setTokens(tokens);
    update();
}