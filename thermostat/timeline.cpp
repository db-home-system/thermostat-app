#include "app-config.h"
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

#define TIMELINE_LABEL_FMT "%1"
#define LABEL_SPACE 65
#define DIVISION_LINE_LABEL_LEN   15
#define DIVISION_LINE_H_LEN       20
#define DIVISION_LINE_MARK_LEN    20
#define START_ANGLE -60
#define ANGLE 300
#define LABEL_SIZE_OFF 7.5
#define ROTATE_ANGLE 12.5

Timeline::Timeline(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    Manager *mgr_class = Manager::instance();
    connect(mgr_class, &Manager::timelineChanged, this, &Timeline::updateTimeline);
    connect(mgr_class, &Manager::currentHour, this, &Timeline::updateTimelineMark);
    connect(mgr_class, &Manager::currentStatus, this, &Timeline::updateThermoStatus);

    // Timeline marker, show the current hour slot
    _current_h = 0;
    _thermo_status = 0;

    time_slots = QVector<TimelineSlotsData>(TIMELINE_DIVISION, {0, 15000});
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
    QPen timeLineMark("#FFD700");

    penMain.setWidth(4);
    penMain.setCapStyle(Qt::FlatCap);

    penMoon.setWidth(8);
    penMoon.setCapStyle(Qt::FlatCap);

    penSun.setWidth(8);
    penSun.setCapStyle(Qt::FlatCap);

    timeLineSteps.setWidth(3);
    timeLineSteps.setCapStyle(Qt::FlatCap);

    timeLineMark.setWidth(3);
    timeLineMark.setCapStyle(Qt::RoundCap);

    timeLine.setWidth(2);
    timeLine.setCapStyle(Qt::FlatCap);

    qreal mark_rect_margin_up = timeLineMark.widthF() + LABEL_SPACE - DIVISION_LINE_MARK_LEN;
    qreal sun_rect_margin = penSun.widthF() + LABEL_SPACE;
    qreal main_rect_margin = sun_rect_margin + penMain.widthF() + penSun.widthF() + 1;
    qreal moon_rect_margin = main_rect_margin + penMoon.widthF() + penMain.widthF();
    qreal mark_rect_margin = moon_rect_margin + timeLineMark.widthF() + 12;

    qreal rtSun = sun_rect_margin / 2.0;
    qreal rbSun = bounds.width() - sun_rect_margin;

    qreal rtMain = main_rect_margin / 2.0;
    qreal rbMain = bounds.width() - main_rect_margin;

    qreal rtMoon = moon_rect_margin / 2.0;
    qreal rbMoon = bounds.width() - moon_rect_margin;

    qreal rtMark_up = mark_rect_margin_up / 2.0;
    qreal rbMark_up = bounds.width() - mark_rect_margin_up;

    qreal rtMark = mark_rect_margin / 2.0;
    qreal rbMark = bounds.width() - mark_rect_margin;

    QLine division_label_line(0, rbMoon/2-penMain.widthF(), 0, rbMain/2 + DIVISION_LINE_LABEL_LEN);
    QLine division_h_line(0, rbMoon/2-penMain.widthF(), 0, rbMain/2 + DIVISION_LINE_H_LEN);
    QLine division_mark_line(0, rbMoon/2-penMoon.widthF(), 0, rbMain/2 + DIVISION_LINE_MARK_LEN);


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

    // Timeline Mark Rectangle
    QRectF rectMark_up = QRectF(rtMark_up, rtMark_up, rbMark_up, rbMark_up);
    QRectF rectMark_down = QRectF(rtMark, rtMark, rbMark, rbMark);

    QRectF rectMain = QRectF(rtMain, rtMain, rbMain, rbMain);
    rectMain.moveCenter(bounds.center());

    QRectF rectMoon = QRectF(rtMoon, rtMoon, rbMoon, rbMoon);
    rectMoon.moveCenter(bounds.center());
    painter->setRenderHint(QPainter::Antialiasing);

    // Put background to timeline.
    QPainterPath fillmain;
    fillmain.arcMoveTo(rectMain, -60);
    fillmain.arcTo(rectMain, -60, 300);

    QBrush bg = QBrush("#427df4");
    if (_thermo_status == 1)
        bg = QBrush("#f48342");

    painter->fillPath(fillmain, bg);

    QPainterPath pathSun, pathMoon, pathMark;
    qreal startAngle = 240; //START_ANGLE;
    qreal angle = ROTATE_ANGLE;
    for (int j = 0; j < time_slots.size(); ++j) {
        startAngle = 240 - (angle * j);

        if (time_slots[j].onOff) {
            pathSun.arcMoveTo(rect, startAngle);
            pathSun.arcTo(rect, startAngle, -1 * angle);
        } else {
            pathMoon.arcMoveTo(rectMoon, startAngle);
            pathMoon.arcTo(rectMoon, startAngle,-1 * angle);
        }

        if (j == _current_h) {
            pathMark.arcMoveTo(rectMark_up, startAngle);
            pathMark.arcTo(rectMark_up, startAngle, -1 * angle);

            pathMark.arcMoveTo(rectMark_down, startAngle);
            pathMark.arcTo(rectMark_down, startAngle, -1 * angle);
        }

    }

    painter->setPen(penSun);
    painter->drawPath(pathSun);
    painter->setPen(penMoon);
    painter->drawPath(pathMoon);
    painter->setPen(timeLineMark);
    painter->drawPath(pathMark);

    // Main circle
    QPainterPath pathMain;
    painter->setPen(penMain);
    pathMain.arcMoveTo(rectMain, START_ANGLE);
    pathMain.arcTo(rectMain, START_ANGLE, ANGLE);
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

        if (j == (24 - _current_h) ||
                j == (23 - _current_h))
        {
            painter->setPen(timeLineMark);
            painter->drawLine(division_mark_line);
        }

        painter->rotate(-ROTATE_ANGLE);
    }

}

void Timeline::updateTimeline(QVector<TimelineSlotsData> &p)
{
    time_slots.resize(p.size());
    for (int i = 0; i < p.size(); i++)
        time_slots[i].onOff = p.at(i).onOff;

    update();
}

void Timeline::updateTimelineMark(int h)
{
    if (_current_h != h)
        _current_h = h;
    update();
}

void Timeline::updateThermoStatus(int status)
{
    if (_thermo_status != status)
        _thermo_status = status;
    update();
}
