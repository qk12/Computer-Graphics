#ifndef LINE_H
#define LINE_H

#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>

class Line
{
public:
    void mousePressEvent(QMouseEvent *event);//鼠标单击事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件

    //void drawline(QPainter &painter, int x1,int y1, int x2 ,int y2,int method,int width,int type);
    void drawLine(QPainter &painter);

    void DDALine(int x0, int y0, int x1, int y1);
    void MidpointLine(int x0, int y0, int x1, int y1);
    void BresenhamLine(int x0, int y0, int x1, int y1);

    void Adjust(QPainter &painter,int width,int type);

    void drawPoints(QPainter &painter);
    void drawPolygon(QPainter &painter);

    void init();

    QVector<QPoint> V;
    QVector<QPoint> drawpoints;
    int idx;
    bool Press , Move;
    bool isVertex;
    bool colseFlag;
    QPoint currentPoint;


};
#endif // LINE_H
