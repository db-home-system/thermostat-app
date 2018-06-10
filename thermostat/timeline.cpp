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
};


struct TimeLineLabel {
    int hour;
    int radius;
};

#define TIMELINE_LEN sizeof(hour)/sizeof(struct TimeLineLabel)
struct TimeLineLabel hour[] = {
    {0,  35},
    {3,  30},
    {6,  30},
    {9,  30},
    {12, 25},
    {15, 30},
    {18, 35},
    {21, 35},
    {24, 35}
};

#define LABEL_SPACE 65
#define DIVISION_LINE_LABEL_LEN   15
#define DIVISION_LINE_H_LEN       20
#define START_ANGLE -60
#define ANGLE 300
#define LABEL_SIZE_OFF 7.5
#define ROTATE_ANGLE 12.5
#define TIMELINE_DIVISION 24
#define TIMELINE_LABEL_FMT "%1"

Timeline::Timeline(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void Timeline::paint(QPainter *painter)
{
    if (width() <= 0 || height() <= 0)
        return;

    const QRectF bounds = boundingRect();

    QPen penMain("green");
    QPen penMoon("blue");
    QPen penSun("red");

    QPen timeLine("#8c8a7f");
    QPen timeLineSteps("#800000");

    penMain.setWidth(4);
    penMain.setCapStyle(Qt::RoundCap);

    penMoon.setWidth(8);
    penMoon.setCapStyle(Qt::FlatCap);

    penSun.setWidth(8);
    penSun.setCapStyle(Qt::FlatCap);

    timeLineSteps.setWidth(3);
    timeLineSteps.setCapStyle(Qt::RoundCap);

    timeLine.setWidth(2);
    timeLine.setCapStyle(Qt::RoundCap);

    qreal sun_rect_margin = penSun.widthF() + LABEL_SPACE;
    qreal main_rect_margin = sun_rect_margin + penMain.widthF() + penSun.widthF() + 1;
    qreal moon_rect_margin = main_rect_margin + penMoon.widthF() + LABEL_SPACE;

    qreal rtSun = sun_rect_margin / 2.0 + 1;
    qreal rbSun = bounds.width() - sun_rect_margin - 2;
    qreal rtMain = main_rect_margin / 2.0 + 1;
    qreal rbMain = bounds.width() - main_rect_margin - 2;
    qreal rtMoon = moon_rect_margin / 2.0 + 1;
    qreal rbMoon = bounds.width() - moon_rect_margin - 2;

    QLine division_label_line(0, rbMain/2, 0, rbMain/2 + DIVISION_LINE_LABEL_LEN);
    QLine division_h_line(0, rbMain/2, 0, rbMain/2 + DIVISION_LINE_H_LEN);

    // Main Rectangle
    QRectF rect = QRectF(rtSun, rtSun, rbSun, rbSun);
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

    QRectF rectMain = QRectF(rtMain, rtMain, rbMain, rbMain);
    rectMain.moveCenter(bounds.center());

    QRectF rectMoon = QRectF(rtMoon, rtMoon, rbMoon, rbMoon);
    rectMoon.moveCenter(bounds.center());

    painter->setRenderHint(QPainter::Antialiasing);

    // Main circle
    QPainterPath pathMain;
    painter->setPen(penMain);
    pathMain.arcMoveTo(rectMain, START_ANGLE);
    pathMain.arcTo(rectMain, START_ANGLE, ANGLE);
    painter->fillPath(pathMain, QBrush("#CCFFCC00"));
    painter->drawPath(pathMain);

    // Translate origin to compensate label size
    painter->translate(bounds.center().x() - LABEL_SIZE_OFF, bounds.center().y());

    for (size_t j = 0; j < TIMELINE_LEN; ++j) {
        qreal px = (rbMain/2 + hour[j].radius) * std::sin(((-ROTATE_ANGLE*hour[j].hour+(START_ANGLE/2))*M_PI*2)/360);
        qreal py = (rbMain/2 + hour[j].radius) * std::cos(((-ROTATE_ANGLE*hour[j].hour+(START_ANGLE/2))*M_PI*2)/360);
        painter->drawText(QPoint(px,py), QString(TIMELINE_LABEL_FMT).arg(hour[j].hour));
    }

    painter->translate(LABEL_SIZE_OFF, 0);
    painter->rotate(START_ANGLE/2);
    for (int j = 0; j < TIMELINE_DIVISION + 1; ++j) {
        if (!(j%3)) {
            painter->setPen(timeLineSteps);
            painter->drawLine(division_h_line);
        } else{
            painter->setPen(timeLine);
            painter->drawLine(division_label_line);
        }
        painter->rotate(-ROTATE_ANGLE);
    }


// QPainterPath pathMoon, pathSun, pathMain;
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
}

