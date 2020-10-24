#include "line.h"
#include <cmath>

void Line::init()
{
    V.clear();
    drawpoints.clear();
    idx=-1;
    Press = Move = false;
    isVertex = false;
    colseFlag=false;
}

void Line::mousePressEvent(QMouseEvent *event)
{
    if(V.size()==0)
    {
        V.push_back(event->pos());
        return;
    }
    for (int i = 0; i < V.size(); ++i)
        if (abs(event->pos().x() - V[i].x()) <= 5 && abs(event->pos().y() - V[i].y()) <= 5)
        { //判断鼠标的位置
            idx = i;
            Press = true;
            isVertex = true;
            break;
        }
}

void Line::mouseMoveEvent(QMouseEvent *event)
{
    Move = true;

    int x0 = event->pos().x(), y0 = event->pos().y();
    currentPoint = event->pos();

    isVertex = false;
    for (int i = 0; i < V.size(); ++i)
        if (abs(x0 - V[i].x()) <= 5 && abs(y0 - V[i].y()) <= 5)
            isVertex = true;

    if (Press && colseFlag) //改变点的位置
        V[idx] = QPoint(x0,y0);
}

void Line::mouseReleaseEvent(QMouseEvent *event)
{
    if(Move && !colseFlag)
        V.push_back(event->pos());
    if(V.size()==2)
        colseFlag=true;

    Move = false;
    Press = false;
}

void Line::drawPoints(QPainter &painter)
{
    for (auto&it:V)
        painter.drawPoint(it);
}

void Line::drawLine(QPainter &painter)
{
    if(!colseFlag)
    {
        if(idx!=-1)
            painter.drawLine(V[0],currentPoint);
    }
    else
    {
        int x0=V[0].x(),y0=V[0].y();
        int x1=V[1].x(),y1=V[1].y();
        drawpoints.clear();
        DDALine(x0,y0,x1,y1);
        for(auto&it:drawpoints)
            painter.drawPoint(it);
    }
}

void Line::DDALine(int x0, int y0, int x1, int y1)
{
    float dx = x1 - x0;
    float dy = y1 - y0;
    float k = dy / dx;
    float y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        drawpoints.push_back(QPoint(x,int(y+0.5)));
        y += k; //x每递增1时，y递增k
    }
}

void Line::MidpointLine(int x0, int y0, int x1, int y1)
{
    int a = y0 - y1;
    int b = x1 - x0;
    bool flag=(a*b<=0?true:false);
    int d = (flag?a + a + b:a+a-b);
    int delta1 = (flag?a + a:a+a-b-b);
    int delta2 = (flag?a + a + b + b:a+a);
    int x = x0, y = y0;
    drawpoints.push_back(QPoint(x,y));
    while (x < x1)
    {
        ++x;
        if (d < 0)
        {
            if(flag)
                ++y;
            d += delta2;
        }
        else
        {
            if(!flag)
                --y;
            d += delta1;
        }
        drawpoints.push_back(QPoint(x,y));
    }
}

void Line::BresenhamLine(int x0, int y0, int x1, int y1)
{
    drawpoints.clear();
    float k=0;
    if(x0==x1)
    {
        if(y0>y1)
            qSwap(y0,y1);
        for(int i=y0;i<=y1;++i)
            drawpoints.push_back(QPoint(x0,i));
        return;
    }
    else
        k =  (float)(y1 - y0) / (x1 - x0);

    bool flag=(fabs(k)<=1.0?true:false);
    if(!flag)
        qSwap(x0,y0),qSwap(x1,y1);
    if(x0>x1)
        qSwap(x0,x1),qSwap(y0,y1);

    int dx = x1 - x0;
    int dy = y1 - y0;
    int e = -dx;
    int x = x0, y = y0;
    int ux = (dx > 0 ?1:-1);
    int uy = (dy > 0 ?1:-1);
    dx=abs(dx),dy=abs(dy);
    for (int i = 0; i <= dx; ++i)
    {
        drawpoints.push_back(QPoint(x,y));
        x += ux;
        e += dy + dy;
        if (e >= 0)
        {
            y += uy;
            e -= (dx + dx);
        }
    }

    if(!flag)
        for(int i=0;i<drawpoints.size();++i)
            qSwap(drawpoints[i].rx(),drawpoints[i].ry());

}
