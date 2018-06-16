#ifndef TIMELINE_H
#define TIMELINE_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QVector>

#define TIMELINE_DIVISION 24
#define TIMELINE_LABEL_FMT "%1"

class Timeline : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit Timeline(QQuickItem *parent = nullptr);

private:
    void paint(QPainter *painter);
    QVector<int> time_slots;

public slots:
    void updateTimeline(QVector<int> *p);
};

#endif // TIMELINE_H
