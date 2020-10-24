#ifndef CURVE_H
#define CURVE_H

#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>

class Curve
{
public:
    void mousePressEvent(QMouseEvent *event);//鼠标单击事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件

    void drawBezier(QPainter &painter);
    void drawnode(QVector<QPoint>& nodes,QPainter &painter);
    QPoint getBezierPoint(QPoint a,QPoint b,double t);
    int fact(int n);    //阶乘函数
    void drawFirstMovingCurve(QPainter &painter);
    void drawCurve(QPainter &painter);

    void  recursive_bezier(QPainter &painter,double t);

    void init();

    QVector<QPoint> ctrlPoints;
    QVector<QPoint> bezierNodes;
    double t;

    bool isSettingPoints;   //正在设置控制点
    bool isFirstDrawing;    //首次绘制时动态显示
    int indexChange; //当前正在操作的曲线的控制顶点
    bool done;

    void drawPoint(QPainter &painter,QPoint it, QColor T);
    int idx;
    bool Press;

};


#endif // CURVE_H
