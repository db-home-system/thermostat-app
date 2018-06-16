#include "timeline.h"
#include "manager.h"

#include <QPainter>
#include <QQuickItem>
#include <qmath.h>

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

Timeline::Timeline(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    Manager *mgr = Manager::instance();
    connect(mgr, SIGNAL(timelineChanged(QVector<int>*)),
            this, SLOT(updateTimeline(QVector<int>*)));
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
    penMain.setCapStyle(Qt::FlatCap);

    penMoon.setWidth(8);
    penMoon.setCapStyle(Qt::FlatCap);

    penSun.setWidth(8);
    penSun.setCapStyle(Qt::FlatCap);

    timeLineSteps.setWidth(3);
    timeLineSteps.setCapStyle(Qt::FlatCap);

    timeLine.setWidth(2);
    timeLine.setCapStyle(Qt::FlatCap);

    qreal sun_rect_margin = penSun.widthF() + LABEL_SPACE;
    qreal main_rect_margin = sun_rect_margin + penMain.widthF() + penSun.widthF() + 1;
    qreal moon_rect_margin = main_rect_margin + penMoon.widthF() + penMain.widthF();

    qreal rtSun = sun_rect_margin / 2.0;
    qreal rbSun = bounds.width() - sun_rect_margin;
    qreal rtMain = main_rect_margin / 2.0;
    qreal rbMain = bounds.width() - main_rect_margin;
    qreal rtMoon = moon_rect_margin / 2.0;
    qreal rbMoon = bounds.width() - moon_rect_margin;

    QLine division_label_line(0, rbMoon/2-penMain.widthF(), 0, rbMain/2 + DIVISION_LINE_LABEL_LEN);
    QLine division_h_line(0, rbMoon/2-penMain.widthF(), 0, rbMain/2 + DIVISION_LINE_H_LEN);

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

    //painter->translate(0, 20);

    QRectF rectMoon = QRectF(rtMoon, rtMoon, rbMoon, rbMoon);
    rectMoon.moveCenter(bounds.center());
    painter->setRenderHint(QPainter::Antialiasing);

    // Put background to timeline.
    QPainterPath fillmain;
    fillmain.arcMoveTo(rectMain, -60);
    fillmain.arcTo(rectMain, -60, 300);
    painter->fillPath(fillmain, QBrush("#00ff33"));

    QPainterPath pathSun, pathMoon;
    qreal startAngle = START_ANGLE;
    qreal angle = ROTATE_ANGLE;
    for (int j = 0; j < time_slots.size(); ++j) {
        startAngle = (angle * j) + START_ANGLE;
        if (time_slots[j]) {
            pathSun.arcMoveTo(rect, startAngle);
            pathSun.arcTo(rect, startAngle, angle);
        } else {
            pathMoon.arcMoveTo(rectMoon, startAngle);
            pathMoon.arcTo(rectMoon, startAngle, angle);
        }
    }
    painter->setPen(penSun);
    painter->drawPath(pathSun);
    painter->setPen(penMoon);
    painter->drawPath(pathMoon);

    // Main circle
    QPainterPath pathMain;
    painter->setPen(penMain);
    pathMain.arcMoveTo(rectMain, START_ANGLE);
    pathMain.arcTo(rectMain, START_ANGLE, ANGLE);
    //painter->fillPath(pathMain, QBrush("#CCFFCC00"));
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
}

void Timeline::updateTimeline(QVector<int> *p)
{
    time_slots.resize(p->size());
    for (int i = 0; i < p->size(); i++)
        time_slots[i] = p->at(i);
    update();
}

