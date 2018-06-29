#ifndef TIMELINE_H
#define TIMELINE_H

#include "manager.h"

#include <QQuickPaintedItem>
#include <QColor>
#include <QVector>

class Timeline : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit Timeline(QQuickItem *parent = nullptr);

private:
    void paint(QPainter *painter);
    QVector<TimelineSlotsData> time_slots;

    int _current_h;

public slots:
    void updateTimeline(QVector<TimelineSlotsData> &p);
    void updateTimelineMark(int h);
};

#endif // TIMELINE_H
