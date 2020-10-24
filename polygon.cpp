#include "polygon.h"
#include <algorithm>
#include <QDebug>

void Polygon::init()
{
    vertices.clear();
    drawpoints.clear();

    colseFlag=false;
    done = false;
    V.clear();
    V1.clear();
    idx=-1;
    Press = Move = false;
    isVertex=false;
}

void Polygon::mousePressEvent(QMouseEvent *event)
{
    if(V.size()==0)
    {
        V.push_back(event->pos());
        return;
    }
    if(colseFlag && V1.size()==0)
    {
        V1.push_back(event->pos());
        return;
    }
    for (int i = 0; i < V.size(); ++i)
        if (abs(event->pos().x() - V[i].x()) <= 5 && abs(event->pos().y() - V[i].y()) <= 5)
        { //判断鼠标的位置
            idx = i;
            Press = true;
            isVertex = true;
            return;
        }
    for (int i = 0; i < V1.size(); ++i)
        if (abs(event->pos().x() - V1[i].x()) <= 5 && abs(event->pos().y() - V1[i].y()) <= 5)
        { //判断鼠标的位置
            idx = i;
            Press = true;
            isVertex = true;
            return;
        }
}

void Polygon::mouseMoveEvent(QMouseEvent *event)
{
    Move = true;

    int x0 = event->pos().x(), y0 = event->pos().y();
    currentPoint = event->pos();

    isVertex = false;
    for (int i = 0; i < V.size(); ++i)
        if (abs(x0 - V[i].x()) <= 5 && abs(y0 - V[i].y()) <= 5)
            isVertex = true;

    if (Press && done) //改变点的位置
        V[idx] = QPoint(x0,y0);
}

void Polygon::mouseReleaseEvent(QMouseEvent *event)
{
    if(Move &&!colseFlag)
    {
        for(auto&it:V)
            if (abs(event->pos().x() - it.x()) <= 5 && abs(event->pos().y() - it.y()) <= 5)
            {
                colseFlag = true;
                idx=-1;
                Press = Move=false;
                isVertex=false;
            }
    }

    if(Move &&colseFlag && !done)
    {
        for(auto&it:V1)
            if (abs(event->pos().x() - it.x()) <= 5 && abs(event->pos().y() - it.y()) <= 5)
            {
                done = true;
                idx=-1;
                Press = Move=false;
                isVertex=false;
            }
    }


    if(Move && !colseFlag)
        V.push_back(event->pos());
    else if(Move && !done)
        V1.push_back(event->pos());

    Move = false;
    Press = false;
}

void Polygon::drawPoints(QPainter &painter)
{
    for (auto&it:V)
        painter.drawPoint(it);
    for (auto&it:V1)
        painter.drawPoint(it);
}

void Polygon::drawPolygon(QPainter &painter)
{
    for(int i=0;i<V.size()-1;++i)
        painter.drawLine(V[i],V[i+1]);
    if(colseFlag)
        painter.drawLine(V[V.size()-1],V[0]);

    //if(idx!=-1)
        //painter.drawLine(V[idx],currentPoint);

    for(int i=0;i<V1.size()-1;++i)
        painter.drawLine(V1[i],V1[i+1]);
    if(done)
        painter.drawLine(V1[V1.size()-1],V1[0]);

    //qDebug()<<V.size()<<' '<<V1.size();

    /*
    else
    {        //多边形
        drawpoints.clear();
        vertices=V;
        polygonScan();
        for(auto&it:drawpoints)
            painter.drawPoint(it);
    }*/
}
