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

    Q_PROPERTY(int timelineSlots READ getTimelineSlots NOTIFY timelineSlotsChanged)

public:
    explicit Timeline(QQuickItem *parent = nullptr);
    int getTimelineSlots() const;
    //void setTimelineSlots(int index, int status);

public slots:
    void setParm(int index, int status);
signals:
    void timelineSlotsChanged(int index, int status);

private:
    void paint(QPainter *painter);
    timeslot_t timeline_slots[TIMELINE_DIVISION];
};

#endif // TIMELINE_H
