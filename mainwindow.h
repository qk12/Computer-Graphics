#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "polygon.h"
#include "line.h"
#include "fill.h"
#include "cut.h"
#include <vector>
#include "curve.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const double pi = 3.14159265358;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void paintEvent(QPaintEvent *event);  //绘图事件
    void mousePressEvent(QMouseEvent *event);//鼠标单击事件
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件
    void mouseDoubleClickEvent(QMouseEvent *event);//鼠标双击事件

    void setPolygonModel();
    void setLineModel();
    void setFillerModel();
    void setCutModel();
    void setCurveModel();
    void Clear();

    void translate(QVector<QPoint> &vertices);
    void rotate(QVector<QPoint> &vertices);
    void scale(QVector<QPoint> &vertices);

    void animeDraw(QPainter &painter);
    void drawCurve(QPainter &painter);

    bool test = false;
    Cut C;
    Line L;
    Polygon Ply;
    Curve curve;
    bool isVertex=false;
    QString Model;
    Fill F;
    QPoint cutStart, cutEnd;

public slots:
    void Translate();
    void Rotate();
    void Scale();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
