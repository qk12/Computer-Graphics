#include "curve.h"
#include <algorithm>
#include <QDebug>
#include <QPaintEvent>
#include <QMainWindow>

void Curve::init()
{
    Press = false;
    done = false;
    idx = -1;
    t = 0;
    ctrlPoints.clear();
    bezierNodes.clear();
}


void Curve::mousePressEvent(QMouseEvent *event)
{
    if(!done)
        ctrlPoints.push_back(event->pos());
    else
    {
        for (int i = 0; i < ctrlPoints.size(); ++i)
            if (abs(event->pos().x() - ctrlPoints[i].x()) <= 5 && abs(event->pos().y() - ctrlPoints[i].y()) <= 5)
            { //判断鼠标的位置
                idx = i;
                Press = true;
                return;
            }
    }
}

void Curve::mouseMoveEvent(QMouseEvent *event)
{
    if (Press) //改变点的位置
        ctrlPoints[idx] = event->pos();
}

void Curve::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    Press = false;
}

void Curve::drawPoint(QPainter &painter,QPoint it, QColor T)
{
    QPen debugPen;
    debugPen.setWidth(1);
    debugPen.setColor(T); //颜色
    painter.setPen(debugPen);
    int ridus = 6;
    for (int i = it.x() - ridus; i < it.x() + ridus; ++i)
        for (int j = it.y() - ridus; j < it.y() + ridus; ++j)
            if ((it.x()-i)*(it.x()-i)+(it.y()-j)*(it.y()-j) <= ridus*ridus)
            {
                painter.drawPoint(i, j);
            }
}

void  Curve::recursive_bezier(QPainter &painter,double t)
{
    Q_UNUSED(painter);
    int n = ctrlPoints.size(); //计算控制点个数
    QVector<QPoint> pp = ctrlPoints;             //压缩存储计算点
    int s = 0;
    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            pp.push_back(QPoint((1.0 - t) * pp[s + j].x() + t * pp[s + j + 1].x(),
                                (1.0 - t) * pp[s + j].y() + t * pp[s + j + 1].y()));
        }
        s = pp.size() - n + i;
    }
    bezierNodes.push_back(pp.back());
}
