#include "timeline.h"


#include <QPainter>
#include <QQuickItem>
#include <qmath.h>

static int item[] = {
            0, // 0
            1, // 1
            0, // 2
            1, // 3
            0, // 4
            1, // 5
            0, // 6
            1, // 7
            0, // 8
            1, // 9
            0, // 10
            1, // 11

            0, // 0
            1, // 1
            0, // 2
            1, // 3
            0, // 4
            1, // 5
            0, // 6
            1, // 7
            0, // 8
            1, // 9
            0, // 10
            1, // 11

            0, // 11
        };

Timeline::Timeline(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void Timeline::paint(QPainter *painter)
{
    if (width() <= 0 || height() <= 0)
        return;

    const QRectF bounds = boundingRect();

    QPen penSun("red");
    penSun.setWidth(8);
    penSun.setCapStyle(Qt::FlatCap);

    qreal a = penSun.widthF() + 50;
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

    a = penMain.widthF() + penMain.widthF() + 9 + 50;
    QRectF rectMain = QRectF(a / 2.0 + 1, a / 2.0 + 1,
                         bounds.width() - a - 2,
                         bounds.height() - a - 2);

    rectMain.moveCenter(bounds.center());

    QPen penMoon("blue");
    penMoon.setWidth(8);
    penMoon.setCapStyle(Qt::FlatCap);

    a = penMoon.widthF() + penMain.widthF() +  penSun.widthF() + 9 + 50;
    QRectF rectMoon = QRectF(a / 2.0 + 1, a / 2.0 + 1,
                         bounds.width() - a - 2,
                         bounds.height() - a - 2);

    rectMoon.moveCenter(bounds.center());

    painter->setRenderHint(QPainter::Antialiasing);

    QPainterPath pathMoon, pathSun, pathMain;


    painter->setPen(penMain);

    // Main circle
    pathMain.arcMoveTo(rectMain, -60);
    pathMain.arcTo(rectMain, -60, 300);
    painter->fillPath(pathMain, QBrush("#CCFFCC00"));
    painter->drawPath(pathMain);

//    int startAngle = -60;
//    qreal angle = 12.5;
//    painter->setPen(penMoon);
//    for (int i = 0; i <= 25; i++){
//        pathMoon.arcMoveTo(rectMoon, startAngle);
//        if (item[i]) {
//            pathMoon.arcTo(rectMoon, startAngle, angle);
//            startAngle = (angle* i) - 60;
//        } else {
//            startAngle = (angle * i) - 60;
//        }
//    }

//    painter->drawPath(pathMoon);

//    startAngle = -60;
//    angle = 25;
//    painter->setPen(penSun);
//    for (int i = 0; i <= 12; i++){
//        pathSun.arcMoveTo(rect, startAngle);
//        if (item[i]) {
//            pathSun.arcTo(rect, startAngle, angle);
//        }
//        startAngle = (angle * i) - 60;
//        qDebug() << startAngle;
//    }
//    painter->drawPath(pathSun);




    painter->translate(bounds.center().x() - 15, bounds.center().y());

    struct TimeLineLabel {
        int hour;
        int radius;
    };

    struct TimeLineLabel hour[] = {
        {0,120},
        {3,125},
        {6,120},
        {9,120},
        {12,110},
        {15,120},
        {18,120},
        {21,125},
        {24,120}
    };
#define TIMELINE_LEN sizeof(hour)/sizeof(struct TimeLineLabel)
    qDebug() << TIMELINE_LEN;
    for (size_t j = 0; j < TIMELINE_LEN; ++j) {
        qreal px = hour[j].radius * std::sin(((-12.5*hour[j].hour-30)*M_PI*2)/360);
        qreal py = hour[j].radius * std::cos(((-12.5*hour[j].hour-30)*M_PI*2)/360);
        painter->drawText(QPoint(px,py), QString("%1:00").arg(hour[j].hour));
    }


    QPen ll("#8c8a7f");
    ll.setWidth(2);
    ll.setCapStyle(Qt::RoundCap);

    QPen lh("#800000");
    lh.setWidth(3);
    lh.setCapStyle(Qt::RoundCap);

    painter->translate(15, 0);
    painter->rotate(-30);
    //painter->drawLine(0,bounds.height() - 170,0,bounds.height() - 150);
    for (int j = 0; j < 25; ++j) {
        if (!(j%3)) {
            painter->setPen(lh);
            painter->drawLine(0,bounds.height() - 170,0,bounds.height() - 145);
        } else{
            painter->setPen(ll);
            painter->drawLine(0,bounds.height() - 170,0,bounds.height() - 150);
        }
        painter->rotate(-12.5);
    }
}

