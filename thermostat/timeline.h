#ifndef TIMELINE_H
#define TIMELINE_H

#include <QQuickPaintedItem>
#include <QColor>

class Timeline : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit Timeline(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
    void setClipRect(QPainter *painter);
};

#endif // TIMELINE_H
