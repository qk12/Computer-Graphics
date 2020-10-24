#ifndef FILL_H
#define FILL_H

#include <QPainter>
#include <QVector>
#include "line.h"

struct Edge
{
    int ymax; //边的上端点y坐标
    float x;  //初值为边下端点的x坐标，AEL中为当前扫描线与边交点的x坐标
    float deltax; //边的斜率的倒数
    Edge *next;   //指向下一条边的指针
};


class Fill
{
public:
    void seedfill4(int x,int y);
    void polygonScan();
    void drawLine(QPainter &painter);


    QVector<QPoint> linePoints;
    bool seed;
    QPoint start;

    QVector<QPoint> V;
    QVector<QPoint> V1;
    QVector<QPoint> drawpoints;
    Line L;
    bool draw[960][600];
    bool check(int x,int y);


    void init();
    int cnt=0;

    void fill(QPainter &painter);

    //边表
    Edge *ET[1000];
    //活动边表
    Edge *AET;

};

#endif // FILL_H
