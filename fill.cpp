#include "fill.h"
#include <QDebug>

void Fill::init()
{
    linePoints.clear();
    V.clear();
    V1.clear();
    drawpoints.clear();
    L.init();
    seed=false;
    for(int i=0;i<960;++i)
        for(int j=0;j<600;++j)
            draw[i][j]=false;
}

bool Fill::check(int x, int y)
{
    return 0<x && x<960 && 0<y && y<600;
}

void Fill::seedfill4(int x,int y)
{
    //qDebug()<<'#'<<++cnt<<' '<<drawpoints.size()<<endl;
    if(draw[x][y])
        return;
    draw[x][y] = true;
    drawpoints.push_back(QPoint(x,y));
    if(check(x,y+1))
        seedfill4(x,y+1);
    if(check(x,y-1))
        seedfill4(x,y-1);
    if(check(x+1,y))
        seedfill4(x+1,y);
    if(check(x-1,y))
        seedfill4(x-1,y);
}

void Fill::drawLine(QPainter &painter)
{
    int sz=V.size();
    for(int i=0;i<sz;++i)
    {
        L.BresenhamLine(V[i].x(),V[i].y(),V[(i+1)%sz].x(),V[(i+1)%sz].y());
        for(auto&it:L.drawpoints)
        {
            painter.drawPoint(it);
            draw[it.x()][it.y()]=true;
        }
    }
    int sz1=V1.size();
    for(int i=0;i<sz1;++i)
    {
        L.BresenhamLine(V1[i].x(),V1[i].y(),V1[(i+1)%sz1].x(),V1[(i+1)%sz1].y());
        for(auto&it:L.drawpoints)
        {
            painter.drawPoint(it);
            draw[it.x()][it.y()]=true;
        }
    }
}

void Fill::fill(QPainter &painter)
{
    for(auto&it:drawpoints)
        painter.drawPoint(it);
}

void Fill::polygonScan()
{ //计算最高点的y坐标

    int maxY = -10000, minY = 10000;
    for (int i = 0; i < V.size(); ++i)
    {
        maxY = std::max(maxY, V[i].y());
        minY = std::min(minY, V[i].y());
    }

    //初始化ET和AET
    for (int i = minY; i < maxY; i++)
    {
        ET[i] = new Edge();
        ET[i]->next = nullptr;
    }
    AET = new Edge();
    AET->next = nullptr;

    //建立边表ET
    int sz=V.size();
    for (int i = 0; i < sz; ++i)
    { //取出当前点1前后相邻的共4个点，点1与点2的连线作为本次循环处理的边，另外两个点点0和点3用于计算奇点
        QPoint P0 = V[(i - 1 + sz) % sz];
        QPoint P1 = V[i];
        QPoint P2 = V[(i + 1) % sz];
        QPoint P3 = V[(i + 2) % sz];

        int x1=P1.x(),y1=P1.y();
        int x2=P2.x(),y2=P2.y();

        //水平线直接舍弃
        if (y1 == y2)
            continue;
        //分别计算下端点y坐标、上端点y坐标、下端点x坐标和斜率倒数
        int ymin = std::min(y1, y2);
        int ymax = std::max(y1, y2);
        float x = y1 > y2 ? x2 : x1;
        float deltax = (float)(x1 - x2) / (y1 - y2);
        //奇点特殊处理，若点2->1->0的y坐标单调递减则y1为奇点，若点1->2->3的y坐标单调递减则y2为奇点
        if (((y1 < y2) && (y1 > P0.y())) || ((y2 < y1) && (y2 > P3.y())))
        {
            ymin++;
            x += deltax;
        }
        //创建新边，插入边表ET
        Edge *p = new Edge();
        p->ymax = ymax;
        p->x = x;
        p->deltax = deltax;
        p->next = ET[ymin]->next;
        ET[ymin]->next = p;
    }

    int sz1=V1.size();
    for (int i = 0; i < sz1; ++i)
    { //取出当前点1前后相邻的共4个点，点1与点2的连线作为本次循环处理的边，另外两个点点0和点3用于计算奇点
        QPoint P0 = V1[(i - 1 + sz1) % sz1];
        QPoint P1 = V1[i];
        QPoint P2 = V1[(i + 1) % sz1];
        QPoint P3 = V1[(i + 2) % sz1];

        int x1=P1.x(),y1=P1.y();
        int x2=P2.x(),y2=P2.y();

        //水平线直接舍弃
        if (y1 == y2)
            continue;
        //分别计算下端点y坐标、上端点y坐标、下端点x坐标和斜率倒数
        int ymin = std::min(y1, y2);
        int ymax = std::max(y1, y2);
        float x = y1 > y2 ? x2 : x1;
        float deltax = (float)(x1 - x2) / (y1 - y2);
        //奇点特殊处理，若点2->1->0的y坐标单调递减则y1为奇点，若点1->2->3的y坐标单调递减则y2为奇点
        if (((y1 < y2) && (y1 > P0.y())) || ((y2 < y1) && (y2 > P3.y())))
        {
            ymin++;
            x += deltax;
        }
        //创建新边，插入边表ET
        Edge *p = new Edge();
        p->ymax = ymax;
        p->x = x;
        p->deltax = deltax;
        p->next = ET[ymin]->next;
        ET[ymin]->next = p;
    }

    //扫描线从下往上扫描，y坐标每次加1
    for (int i = minY; i < maxY; ++i)
    { //取出ET中当前扫描行的所有边并按x的递增顺序（若x相等则按deltax的递增顺序）插入AET
        while (ET[i]->next)
        { //取出ET中当前扫描行表头位置的边
            Edge *pInsert = ET[i]->next;
            Edge *p = AET;
            while (p->next) //在AET中搜索合适的插入位置
            {
                if (pInsert->x > p->next->x)
                {
                    p = p->next;
                    continue;
                }
                if (pInsert->x == p->next->x && pInsert->deltax > p->next->deltax)
                {
                    p = p->next;
                    continue;
                }
                break; //找到位置
            }
            ET[i]->next = pInsert->next; //将pInsert从ET中删除，并插入AET的当前位置
            pInsert->next = p->next;
            p->next = pInsert;
        }

        //AET中的边两两配对并填色
        Edge *p = AET;
        while (p->next && p->next->next)
        {
            for (int x = p->next->x; x < p->next->next->x; ++x)
                drawpoints.push_back(QPoint(x,i));
            p = p->next->next;
        }

        p = AET;
        while (p->next)
        {
            if (p->next->ymax == i) //删除AET中满足y=ymax的边
            {
                Edge *pDelete = p->next;
                p->next = pDelete->next;
                pDelete->next = nullptr;
                delete pDelete;
            }
            else //更新AET中边的x值，进入下一循环
            {
                p->next->x += p->next->deltax;
                p = p->next;
            }
        }
    }
}
