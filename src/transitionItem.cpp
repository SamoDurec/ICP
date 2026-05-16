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
 * @brief Vytvorenie grafickej reprezentácie miesta.
 * 
 * @param transition Objekt reprezentujúci súvisiaci prechod.
 * @param parent Rodičovský objekt.
 */
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

/**
 * @brief Prekreslenie prechodu pri zmene.
 */
void TransitionItem::refresh() {
    update();
}

/**
 * @brief Hlavná vykresľovacia metóda prechodu.
 * 
 * Vykreslí obdĺžnik značiaci prechod a jeho názov.
 * Zmení farbu pri zakliknutí.
 * Mení farbu pri enabled.
 */
void TransitionItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)

    // nastavenie farby podľa stavu
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
    if (isSelected()) painter->setPen(QPen(Qt::gray, 3));

    // vykreslenie obdĺžnika
    painter->drawRect(boundingRect());

    // vykreslenie názvu
    painter->setFont(QFont("Arial", 9, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, m_transition->id());
}

/**
 * @brief Zaisťuje pridanie nových hrán.
 */
void TransitionItem::addArc(ArcItem *arc)
{
    m_arcs.append(arc);
}

/**
 * @brief Hlavná metóda zaisťujúca vykresľovanie prechodov a hrán pri zmene pozície prechodu.
 */
QVariant TransitionItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {

        m_transition->setPos(value.toPointF());

        // nájde všetky pripojené hrany
        for(ArcItem *arc : m_arcs) {
            if (arc) {
                arc->updateGeometry(); // prekreslenie hrán
            }
        }
    }

    return QGraphicsRectItem::itemChange(change, value);
}

/**
 * @brief Zaisťuje editáciu parametrov prechodu.
 * Spustenie dvojklikom na prechod.
 */
void TransitionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
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
    
    if(!ok) return;

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

    if (!ok) return;

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

    if (!ok) return;

    // action
    QString action = QInputDialog::getText(
        nullptr,
        "Edit transition",
        "Action: ",
        QLineEdit::Normal,
        m_transition->action(),
        &ok);

    if (!ok) return;
    
    // aktualizácia parametrov
    m_transition->setId(newId);
    m_transition->setEventName(eventName);
    m_transition->setGuard(guard);
    m_transition->setDelayMs(delay);
    m_transition->setAction(action);

    update();
}