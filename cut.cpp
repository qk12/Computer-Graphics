#include "cut.h"

int Cut::encode(int x, int y)
{
    int code = 0;
    if (x < XL)
        code |= LEFT;
    else if (x > XR)
        code |= RIGHT;
    if (y < YB)
        code |= BOTTOM;
    else if (y > YT)
        code |= TOP;
    return code;
}

bool Cut::CohenSutherland_LineClip(int x1, int y1, int x2, int y2)
{
    int code1 = encode(x1, y1);
    int code2 = encode(x2, y2);
    //若 code1==0 && code2==0 则说明P1和P2均在窗口内，整条线段必在窗口内
    while (code1 != 0 || code2 != 0)
    {
        //若两个端点同在窗口的某一侧，则线段完全在窗口外
        if (code1 & code2)
            return false;

        int x, y; //线段与边界的交点坐标
        int code = (code1 == 0 ? code2 : code1);

        if (LEFT & code) //线段与左边界相交
        {
            x = XL; //计算交点坐标
            y = y1 + (y2 - y1) * (XL - x1) / (x2 - x1);
        }
        else if (RIGHT & code) //线段与右边界相交
        {
            x = XR;
            y = y1 + (y2 - y1) * (XR - x1) / (x2 - x1);
        }
        else if (BOTTOM & code) //线段与下边界相交
        {
            y = YB;
            x = x1 + (x2 - x1) * (YB - y1) / (y2 - y1);
        }
        else if (TOP & code) //线段与上边界相交
        {
            y = YT;
            x = x1 + (x2 - x1) * (YT - y1) / (y2 - y1);
        }
        //用交点坐标值代替被舍弃端点的坐标，即可舍弃窗口外的子线段
        if (code == code1)
        {
            x1 = x, y1 = y;
            code1 = encode(x1, y1);
        }
        else
        {
            x2 = x, y2 = y;
            code2 = encode(x2, y2);
        }
    }
    P0 = QPoint(x1,y1);
    P1 = QPoint(x2,y2);
    return true;
}


bool Cut::MidClip(int x1, int y1, int x2, int y2)
{
    float xa = x1;
    float ya = y1;
    float xb = x2;
    float yb = y2;

    float xm = (xa + xb) / 2;
    float ym = (ya + yb) / 2;

    float l = 0;

    int code1 = encode(xa, ya);
    int code2 = encode(xb, yb);

    if (code1 & code2)
            return false;

    int codem = encode(xm, ym); //计算中点区域码

    while (true)//求（x1,y1）点最近的可见点A
    {
        xm = (xa + xb) / 2;
        ym = (ya + yb) / 2;
        code1 = encode(xa, ya);
        codem = encode(xm, ym);
        if ((code1&codem) == 0)
        {
            xb = xm;
            yb = ym;
            l = sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb));
        }
        else
        {
            xa = xm;
            ya = ym;
            l = sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb));
        }
        if (l <= m) {
            break;
        }
    }
    int Ax = xm;
    int Ay = ym;

    xa = x1;
    ya = y1;
    xb = x2;
    yb = y2;

    while (true)//求（x2,y2）点最近的可见点B
    {
        xm = (xa + xb) / 2;
        ym = (ya + yb) / 2;
        code2 = encode(xb, yb);
        codem = encode(xm, ym);
        if ((code2&codem) == 0)
        {
            xa = xm;
            ya = ym;
            l = sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb));
        }
        else
        {
            xb = xm;
            yb = ym;
            l = sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb));
        }
        if (l <= m) {
            break;
        }
    }
    int Bx = xm;
    int By = ym;
    P0 = QPoint(Ax,Ay);
    P1 = QPoint(Bx,By);
    return true;
}

