#ifndef TIMELINE_H
#define TIMELINE_H

#include <QQuickPaintedItem>
#include <QColor>

#define TIMELINE_DIVISION 24
#define TIMELINE_LABEL_FMT "%1"
typedef struct TimeLineSlots {
   int status;
} timeslot_t;

class Timeline : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit Timeline(QQuickItem *parent = nullptr);

private:
    void paint(QPainter *painter);
    timeslot_t timeline_slots[TIMELINE_DIVISION];
};

#endif // TIMELINE_H
