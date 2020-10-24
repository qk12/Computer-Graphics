#ifndef CUT_H
#define CUT_H

#include <QPoint>
#include <cmath>

class Cut
{
public:
    const int LEFT = 1;   //0001
    const int RIGHT = 2;  //0010
    const int BOTTOM = 4; //0100
    const int TOP = 8;    //1000
    /* 窗口的九个区域
    1001  1000  1010
    0001  0000  0010
    0101  0100  0110
    */

    const float m = 1; // 容差

    int XL, XR, YB, YT;

    int encode(int x, int y);
    bool CohenSutherland_LineClip(int x1, int y1, int x2, int y2);
    bool MidClip(int x1, int y1, int x2, int y2);

    bool cutdone=false;
    bool cutting=false;
    QPoint P0,P1;

};

#endif // CUT_H
