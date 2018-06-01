#include "timeline.h"


#include <QPainter>
#include <QQuickItem>
#include <qmath.h>

#define INITIAL_ANGLE 0.0
#define MAX_ANGLE     360.0

Timeline::Timeline(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{ }

void Timeline::paint(QPainter *painter)
{
    if (width() <= 0 || height() <= 0)
        return;

    const QRectF bounds = boundingRect();

    QPen penSun("red");
    penSun.setWidth(8);
    penSun.setCapStyle(Qt::SquareCap);


    qreal a = penSun.widthF();
    QRectF rect = QRectF(a / 2.0 + 1, a / 2.0 + 1,
                         bounds.width() - a - 2,
                         bounds.height() - a - 2);

    rect.moveCenter(bounds.center());

    // Make sure the arc is aligned to whole pixels.
    if (rect.x() - int(rect.x()) > 0)
        rect.setX(qCeil(rect.x()));
    if (rect.y() - int(rect.y()) > 0)
        rect.setY(qCeil(rect.y()));
    if (rect.width() - int(rect.width()) > 0)
        rect.setWidth(qFloor(rect.width()));
    if (rect.height() - int(rect.height()) > 0)
        rect.setHeight(qFloor(rect.height()));


    QPen penMain("green");
    penMain.setWidth(4);
    penMain.setCapStyle(Qt::RoundCap);

    a = penMain.widthF() + penMain.widthF() + 9;
    QRectF rectMain = QRectF(a / 2.0 + 1, a / 2.0 + 1,
                         bounds.width() - a - 2,
                         bounds.height() - a - 2);

    rectMain.moveCenter(bounds.center());

    QPen penMoon("blue");
    penMoon.setWidth(8);
    penMoon.setCapStyle(Qt::SquareCap);

    a = penMoon.widthF() + penMain.widthF() +  penSun.widthF() + 9;
    QRectF rectMoon = QRectF(a / 2.0 + 1, a / 2.0 + 1,
                         bounds.width() - a - 2,
                         bounds.height() - a - 2);

    rectMoon.moveCenter(bounds.center());

    painter->setRenderHint(QPainter::Antialiasing);

    QPainterPath pathMoon, pathSun, pathMain;


    painter->setPen(penSun);
    pathSun.arcMoveTo(rect, 0);
    pathSun.arcTo(rect, 0, 45);
    painter->drawPath(pathSun);

    painter->setPen(penMain);
    pathMain.arcMoveTo(rectMain, -60);
    pathMain.arcTo(rectMain, -60, 300);
    painter->fillPath(pathMain, QBrush("#CCFFCC00"));
    painter->drawPath(pathMain);

    painter->setPen(penMoon);
    pathMoon.arcMoveTo(rectMoon, 0);
    pathMoon.arcTo(rectMoon, 0, 35);

    painter->drawPath(pathMoon);


}

void Timeline::setClipRect(QPainter *painter)
{
    const QRectF bounds = boundingRect();
    painter->setClipRect(0, bounds.height() - 100, bounds.width(), 100);

}

