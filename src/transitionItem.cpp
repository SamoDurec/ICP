/**
 * @file transitionItem.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie prechodu.
 */

#include "transitionItem.hpp"
#include "arcItem.hpp"
#include <QPainter>
#include <QInputDialog>
#include <QGraphicsSceneMouseEvent>

/**
 * @brief Vytvorenie grafickej reprezentacie prechodu.
 */
TransitionItem::TransitionItem(std::shared_ptr<Transition> transition, QGraphicsItem *parent)
    : QGraphicsRectItem(-W/2, -H/2, W, H, parent)
    , m_transition(transition)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(Qt::black, 2));
}

/**
 * @brief Prekreslenie prechodu pri zmene.
 */
void TransitionItem::refresh() {
    update();
}

/**
 * @brief Hlavna vykreslovacia metoda prechodu.
 * Vykresli obdlznik znamenajuci prechod a jeho nazov.
 * Zmeni farbu pri zakliknuti.
 * Meni farbu pri enabled.
 */
void TransitionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option) Q_UNUSED(widget)

    // nastavenie farby podla stavu
    QColor bg;
    switch (m_state) {
        case State::Enabled:      bg = QColor(255, 255, 160); break; // zlta
        case State::PendingTimer: bg = QColor(180, 220, 255); break; // modra
        default:                  bg = Qt::white;             break;
    }

    // nastavenie farby obrysov
    painter->setBrush(bg);
    painter->setPen(QPen(Qt::black, 2));

    // nastavenie farby obrysov pri zakliknutí
    if (isSelected()) {
        painter->setPen(QPen(Qt::gray, 3));
    }

    // vykreslenie obdlznika
    painter->drawRect(boundingRect());

    // vykreslenie nazvu
    painter->setFont(QFont("Arial", 9, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, m_transition->id());
}

/**
 * @brief Zaistuje pridanie novych hran.
 */
void TransitionItem::addArc(ArcItem *arc) {
    m_arcs.append(arc);
}

/**
 * @brief Hlavna metoda zaistujuca vykreslovanie prechodov a hran pri zmene pozicie prechodu.
 */
QVariant TransitionItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        m_transition->setPos(value.toPointF());

        // najde vsetky pripojene hrany
        for(ArcItem *arc : m_arcs) {
            if (arc) {
                arc->updateGeometry(); // prekreslenie hran
            }
        }
    }

    return QGraphicsRectItem::itemChange(change, value);
}

/**
 * @brief Zaistuje editaciu parametrov prechodu.
 * Spustenie dvojklikom na prechod.
 */
void TransitionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);

    bool ok;

    // id
    QString newId = QInputDialog::getText(
        nullptr,
        "Edit transition",
        "Place transition: ",
        QLineEdit::Normal,
        m_transition->id(),
        &ok);
    
    if(!ok) {
        return;
    }

    // event
    QString eventName = QInputDialog::getText(
        nullptr,
        "Edit transition",
        "Event name: ",
        QLineEdit::Normal,
        m_transition->eventName(),
        &ok);

    if (!ok) return;

    // guard
    QString guard = QInputDialog::getText(
        nullptr,
        "Edit transition",
        "Guard: ",
        QLineEdit::Normal,
        m_transition->guard(),
        &ok);

    if (!ok) {
        return;
    }

    // delay
    int delay = QInputDialog::getInt(
        nullptr,
        "Edit transition",
        "Delay (ms or -1 = none): ",
        m_transition->delayMs(),
        -1,
        999999,
        1,
        &ok);

    if (!ok) {
        return;
    }

    // action
    QString action = QInputDialog::getText(
        nullptr,
        "Edit transition",
        "Action: ",
        QLineEdit::Normal,
        m_transition->action(),
        &ok);

    if (!ok) {
        return;
    }

    // aktualizácia parametrov
    m_transition->setId(newId);
    m_transition->setEventName(eventName);
    m_transition->setGuard(guard);
    m_transition->setDelayMs(delay);
    m_transition->setAction(action);

    update();
}