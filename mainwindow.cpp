#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolTip>
#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>
#include <vector>
#include <QtCore/QThread>

typedef std::vector<double> vec;
typedef std::vector<vec> mat;

mat mul(mat &A, mat &B) //计算A*B
{
    mat C(A.size(), vec(B[0].size())); //A矩阵的行数、B矩阵的列数
    for (int i = 0; i < (int)A.size(); ++i)
        for (int k = 0; k < (int)B.size(); ++k)
            if (A[i][k]) //对稀疏矩阵的优化
                for (int j = 0; j < (int)B[0].size(); ++j)
                    C[i][j] += A[i][k] * B[k][j];
    return C;
}

void MainWindow::drawCurve(QPainter &painter)
{
    curve.bezierNodes.clear();
    for(double t=0; t<1; t+=0.001)
    {
        curve.recursive_bezier(painter,t);
    }
    //qDebug()<<"done"<<endl;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("qk"));

    connect(ui->actionPolygon, &QAction::triggered, this, &MainWindow::setPolygonModel);
    connect(ui->actionLine, &QAction::triggered, this, &MainWindow::setLineModel);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::Clear);
    connect(ui->actionFiller,&QAction::triggered, this, &MainWindow::setFillerModel);
    connect(ui->actionCutter,&QAction::triggered, this, &MainWindow::setCutModel);
    connect(ui->actionCurve,&QAction::triggered, this, &MainWindow::setCurveModel);


    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Translate()));  //display() connect
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Scale()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(Rotate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPolygonModel()
{
    Model = "Polygon";
    Ply.init();
    repaint();
}

void MainWindow::setLineModel()
{
    Model = "Line";
    L.init();
    repaint();
}

void MainWindow::setCutModel()
{
    Model = "Cut";
    C.cutdone =C.cutting= false;
    repaint();
}

void MainWindow::setFillerModel()
{
    Model = "Fill";
    F.init();
    repaint();
}

void MainWindow::setCurveModel()
{
    if(Model == "Curve")
    {
        curve.done = true;
    }
    else
    {
        Model = "Curve";
        curve.init();
    }
    repaint();
}

void MainWindow::Clear()
{
    Model = "";
    repaint();
}

void MainWindow::Translate()
{
    translate(Ply.V);
    repaint();
}
void MainWindow::Rotate()
{
    rotate(Ply.V);
    repaint();
}
void MainWindow::Scale()
{
    scale(Ply.V);
    repaint();
}

void MainWindow::translate(QVector<QPoint> &vertices)//参照点与平移向量
{
    QPoint ref;
    ref.rx() = ui->textEdit->toPlainText().toInt();
    ref.ry() = ui->textEdit_2->toPlainText().toInt();
    int x = ui->textEdit_3->toPlainText().toInt();
    int y = ui->textEdit_4->toPlainText().toInt();
    mat m(3,vec(3));
    m[0][0]=1;
    m[0][2]=x;
    m[1][1]=1;
    m[1][2]=y;
    m[2][2]=1;
    vec point(3);
    for (auto&it:vertices) //遍历每个顶点进行修改
    {
        point[0] = it.x();
        point[1] = it.y();
        point[2] = 1;
        it.rx() = m[0][0] * point[0] + m[0][1] * point[1] + m[0][2] * point[2];
        it.ry() = m[1][0] * point[0] + m[1][1] * point[1] + m[1][2] * point[2];
    }
}

void MainWindow::rotate(QVector<QPoint> &vertices)
{
    QPoint ref;
    ref.rx() = ui->textEdit->toPlainText().toInt();
    ref.ry() = ui->textEdit_2->toPlainText().toInt();
    double angle = ui->textEdit_7->toPlainText().toDouble();

    double rad = (angle / 180) * pi;
    mat m(3, vec(3));
    double cosa = cos(rad);
    double sina = sin(rad);
    m[0][0] = cosa;
    m[0][1] = -sina;
    m[0][2] = ref.x() * (1 - cosa) + ref.y() * sina;
    m[1][0] = sina;
    m[1][1] = cosa;
    m[1][2] = ref.y() * (1 - cosa) - ref.x() * sina;
    m[2][2] = 1;
    vec point(3);
    for (auto&it:vertices) //遍历每个顶点进行修改
    {
        point[0] = it.x();
        point[1] = it.y();
        point[2] = 1;
        it.rx() = m[0][0] * point[0] + m[0][1] * point[1] + m[0][2] * point[2];
        it.ry() = m[1][0] * point[0] + m[1][1] * point[1] + m[1][2] * point[2];
    }
}

void MainWindow::scale(QVector<QPoint> &vertices)//参照点与缩放倍数
{
    QPoint ref;
    ref.rx() = ui->textEdit->toPlainText().toInt();
    ref.ry() = ui->textEdit_2->toPlainText().toInt();
    double sizex = ui->textEdit_5->toPlainText().toDouble();
    double sizey = ui->textEdit_6->toPlainText().toDouble();
    //qDebug()<<sizex<<' '<<sizey<<endl;

    mat m(3,vec(3));
    m[0][0]=sizex;
    m[0][2]=ref.x()*(1-sizex);
    m[1][1]=sizey;
    m[1][2]=ref.y()*(1-sizey);
    m[2][2]=1;
    vec point(3);
    for (auto&it:vertices) //遍历每个顶点进行修改
    {
        point[0] = it.x();
        point[1] = it.y();
        point[2] = 1;
        it.rx() = m[0][0] * point[0] + m[0][1] * point[1] + m[0][2] * point[2];
        it.ry() = m[1][0] * point[0] + m[1][1] * point[1] + m[1][2] * point[2];
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) //左键
    {
        if(Model == "Polygon")
            Ply.mousePressEvent(event);
        else if(Model == "Line")
            L.mousePressEvent(event);
        else if(Model == "Cut")
            cutStart = event->pos(),C.cutting=true;
        else if(Model == "Fill")
            F.seed=true,F.start=event->pos();
        else if(Model == "Curve")
            curve.mousePressEvent(event);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {   //显示鼠标坐标  跟随鼠标用方框的形式
        int x0 = event->pos().x(), y0 = event->pos().y();
        QString coordinates = QString("%1 %2").arg(x0).arg(y0);
        QToolTip::showText(QPoint(event->globalPos()), coordinates, this);

        if(Model == "Polygon")
            Ply.mouseMoveEvent(event),isVertex=Ply.isVertex;
        else if(Model == "Line")
            L.mouseMoveEvent(event),isVertex=L.isVertex;
        else if(Model == "Cut")
            cutEnd = event->pos();
        else if(Model == "Curve")
            curve.mouseMoveEvent(event);

        if (isVertex)
            setCursor(Qt::CrossCursor); //改变鼠标的样式
        else
            setCursor(Qt::ArrowCursor);

        repaint(); //刷新
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(Model == "Polygon")
        Ply.mouseReleaseEvent(event);
    else if(Model == "Line")
        L.mouseReleaseEvent(event);
    else if(Model == "Cut")
        C.cutdone = true,C.cutting=false;
    else if(Model == "Curve")
        curve.mouseReleaseEvent(event);

    setCursor(Qt::ArrowCursor);
    repaint();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    //qDebug()<<Model<<endl;

    QPainter painter(this);
    QPen pen;              //设置点的样式
    pen.setColor(Qt::red); //改变颜色
    pen.setWidth(5);       //改变点的宽度
    painter.setPen(pen);

    if(Model == "Polygon" || Model == "Fill")
        Ply.drawPoints(painter);
    else if(Model == "Line")
        L.drawPoints(painter);

    QPen pen1; //设置线的样式
    pen1.setWidth(2);
    pen1.setColor(Qt::blue);
    painter.setPen(pen1);

    if(Model == "Polygon")
        Ply.drawPolygon(painter);
    else if(Model == "Line")
        L.drawLine(painter);
    else if(Model == "Fill")
    {
        F.V = Ply.V;
        F.V1 = Ply.V1;
        for(auto &it:F.V)
            qDebug()<<it.x()<<' '<<it.y()<<endl;
        F.drawLine(painter);
        if(test)
        {
            F.polygonScan();
            F.fill(painter);
        }
        else
        {
            if(F.seed)
            {
                qDebug()<<F.start.x()<<' '<<F.start.y()<<endl;
                F.seedfill4(F.start.x(),F.start.y());
                F.fill(painter);
            }
        }
        //Model = "Polygon";
    }
    else if(Model == "Cut")
    {
        qDebug()<<C.cutdone<<endl;
        if(!C.cutdone)
        {
            //L.drawLine(painter);
            L.drawLine(painter);
            if(C.cutting)
            {
                QPen pen2; //设置线的样式
                pen2.setWidth(1);
                pen2.setColor(Qt::blue);
                pen2.setStyle(Qt::DashLine);
                painter.setPen(pen2);
                painter.drawRect(cutStart.x(), cutStart.y(), cutEnd.x() - cutStart.x(), cutEnd.y() - cutStart.y());
            }
        }
        if(C.cutdone)
        {
            C.XL=cutStart.x(),C.XR=cutEnd.x();
            C.YB=cutStart.y(),C.YT=cutEnd.y();
            bool flag=false;
            if(test)
                flag = C.CohenSutherland_LineClip(L.V[0].x(),L.V[0].y(),L.V[1].x(),L.V[1].y());
            else
                flag = C.MidClip(L.V[0].x(),L.V[0].y(),L.V[1].x(),L.V[1].y());
            if(flag)
            {
                L.V[0]=C.P0,L.V[1]=C.P1;
                //qDebug()<<C.P0.x() << C.P0.y() <<C.P1.x() << C.P1.y()<<endl;
                painter.setPen(pen);
                L.drawPoints(painter);
                Model = "Line";
                painter.setPen(pen1);
                L.drawLine(painter);
            }

        }
    }
    else if(Model == "Curve")
    {
        for(auto&it:curve.ctrlPoints)
            curve.drawPoint(painter, it, Qt::blue); //顶点，蓝色
        QPen debugPen;
        debugPen.setWidth(1);
        debugPen.setColor(Qt::black); //颜色
        painter.setPen(debugPen);
        for(int i=0; i< (int)curve.ctrlPoints.size()-1; ++i)
            painter.drawLine(curve.ctrlPoints[i], curve.ctrlPoints[i+1]);
        if(curve.done)
        {
            drawCurve(painter);
            for(int i=0; i<(int)curve.bezierNodes.size()-1;++i)
                //painter.drawPoint(curve.bezierNodes[i]);
                painter.drawLine(curve.bezierNodes[i], curve.bezierNodes[i+1]);
        }
    }
}

