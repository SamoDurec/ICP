/**
 * @file arcItem.cpp
 * @authors xdurecs00, xpertod00
 * @brief Implementacia grafickej reprezentacie hrany.
 */

#include "arcItem.hpp"
#include <QPainter>
#include <QtMath>

/**
 * @brief Vytvára grafickú reprezentáciu hrany.
 * 
 * @param from Objekt, z ktorého hrana vychádza.
 * @param to Objekt, do ktorého hrana smeruje.
 * @param weight Váha hrany.
 * @param parent Rodičovský objekt.
 */
ArcItem::ArcItem(QGraphicsItem *from, QGraphicsItem *to,
                 int weight, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
    , m_from(from)
    , m_to(to)
    , m_weight(weight)
{
    setFlag(QGraphicsItem::ItemIsSelectable); // umožňuje zakliknúť hranu
    setZValue(-1); // kreslí sa pod miestami a prechodmi
    setPen(QPen(Qt::white, 1.5));
    updateGeometry();
}

/**
 * @brief Zaisťuje vykreslenie, prípadne prekreslenie hrany.
 */
void ArcItem::updateGeometry()
{
    // nájde stred objektu
    QPointF srcCenter = m_from->mapToScene(m_from->boundingRect().center());
    QPointF dstCenter = m_to->mapToScene(m_to->boundingRect().center());

    // spojnica stredov objektov
    QLineF centerLine(srcCenter, dstCenter);

    // nevykresľuj, pokiaľ je hrana príliš krátka
    if (centerLine.length() < 1.0) return;

    // posunutie koncov čiar na kraj objektov
    QPointF src = edgePoint(m_from, dstCenter);
    QPointF dst = edgePoint(m_to, srcCenter);

    setLine(QLineF(src, dst)); // vytvorí geometriu úsečky medzi bodmi
}

/**
 * @brief Hľadá okraj objektu, kde sa má zakončiť hrana.
 */
QPointF ArcItem::edgePoint(QGraphicsItem *item, const QPointF &to)
{
    // hľadá okraj pre obdĺžnik ohraničujúci objekt v scéne
    QRectF rect = item->sceneBoundingRect(); // nájde obdlžník
    QPointF center = rect.center(); // vypočíta stred objektu
    QLineF line(center, to); // smer hrany

    // vytvorenie hrán objektu
    QList<QLineF> edges = {
        QLineF(rect.topLeft(), rect.topRight()),
        QLineF(rect.topRight(), rect.bottomRight()),
        QLineF(rect.bottomRight(), rect.bottomLeft()),
        QLineF(rect.bottomLeft(), rect.topLeft())
    };

    QPointF intersection;

    // hľadanie priesečníka okraja objektu a hrany
    for (const QLineF &edge : edges)
    {
        auto type = line.intersects(edge, &intersection);

        if (type == QLineF::BoundedIntersection) return intersection;
    }

    return center;
}

/**
 * @brief Vracia oblasť, ktorú objekt zaberá pri vykresľovaní.
 */
QRectF ArcItem::boundingRect() const
{
    return QGraphicsLineItem::boundingRect().adjusted(-20, -20, 20, 20);
}
/**
 * @brief Hlavná vykresľovacia metóda hrany.
 * 
 * Vykresluje čáru, šipku i váhu.
 * Označená hrana je označená šedou farbou.
 * Váha 1 sa nevykresľuje.
 */
void ArcItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    Q_UNUSED(option) Q_UNUSED(widget)

    QLineF l = line(); // načíta aktuálnu geometriu hrany
    if (l.length() < 1.0) return; // kontrola dĺžky

    // nastavuje farbu
    if (isSelected()) painter->setPen(QPen(Qt::gray, 1.5)); // vybraná hrana
    else painter->setPen(QPen(Qt::black, 1.5));

    painter->drawLine(l); // vykreslenie čiary

    // vykreslenie šípky na konci
    const double angle = std::atan2(-l.dy(), l.dx());
    const double sz = 10.0;
    QPointF tip = l.p2();
    QPointF p1 = tip + QPointF(std::cos(angle + M_PI*5/6) * sz,
                               -std::sin(angle + M_PI*5/6) * sz);
    QPointF p2 = tip + QPointF(std::cos(angle - M_PI*5/6) * sz,
                               -std::sin(angle - M_PI*5/6) * sz);
    painter->setBrush(Qt::white); // biela výplň
    painter->drawPolygon(QPolygonF({tip, p1, p2}));

    // zobrazenie váhy hrany (ak je väčšia ako 1)
    if (m_weight > 1) {
        QPointF mid = (l.p1() + l.p2()) / 2.0;
        painter->setPen(Qt::yellow);
        painter->setFont(QFont("Arial", 8));
        painter->drawText(mid + QPointF(4, -4), QString::number(m_weight));
    }
}