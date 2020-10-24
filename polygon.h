#ifndef POLYGON_H
#define POLYGON_H

#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>

class Polygon
{
public:

    void mousePressEvent(QMouseEvent *event);//鼠标单击事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件

    void drawPoints(QPainter &painter);
    void drawPolygon(QPainter &painter);

    QVector<QPoint> vertices;
    QVector<QPoint> drawpoints;

    void init();

    bool colseFlag;
    bool done;
    QVector<QPoint> V;
    QVector<QPoint> V1;
    int idx;
    bool Press,Move;
    QPoint currentPoint;
    bool isVertex;

};
#endif // POLYGON_H
