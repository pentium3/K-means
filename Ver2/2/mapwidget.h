#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QGraphicsView>
#include <QLabel>
#include <QMouseEvent>
#include <QGraphicsItem>

class MapWidget : public QGraphicsView
{
    Q_OBJECT
public:
    MapWidget();
    void readMap();
public slots:
    void slotZoom(int);
private:
    qreal zoom;
};

#endif // MAPWIDGET_H
