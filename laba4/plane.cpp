#include "plane.h"
#include <cmath>
#include <QDebug>

\

Plane::Plane(QSize scrSize, QWidget *parent)
    : QWidget{parent}
{
    scene = new QGraphicsScene(this);
    view = new MyGraphicsView(scene, this);

    int x = scrSize.width() / 2;
    int y = scrSize.height() / 2;
    int div = 30;

    QGraphicsTextItem *text = scene->addText("x");
    text->setPos(x, div);
    text = scene->addText("y");
    text->setPos(-div, -y);

    for (int i = -x; i <= x; i += div)
    {
        text = scene->addText(QString::number(i / div));
        text->setPos(i, 0);
    }

    for (int i = -y; i <= y; i += div)
    {
        text = scene->addText(QString::number(- i / div));
        text->setPos(0, i);
    }

    for (int i = -x; i <= x; i += div)
    {
        for (int h = -y; h <= y; h += div)
        {
            scene->addRect(i, h, div, div, QPen(Qt::gray));
        }
    }


    scene->addLine(-x, 0, x, 0, QPen(Qt::black));
    scene->addLine(0, -y, 0, y, QPen(Qt::black));
    view->setGeometry(QRect({0, 0}, scrSize));
}

void Plane::iterations(QPoint f, QPoint s)
{

    double dx = ((double)s.x()-(double)f.x());
    double dy = ((double)s.y()-(double)f.y());

    scene->addEllipse(f.x() * 30 - 5, -f.y() * 30 - 5, 10, 10, QPen(Qt::green));
    scene->addEllipse(s.x() * 30 - 5, -s.y() * 30 - 5, 10, 10, QPen(Qt::green));


    if(dx == 0 && dy == 0)
    {
        return;
    }

    if(abs(dx)>=abs(dy)){
        double a = dy/dx;
        double x,y;

        if(f.x()<=s.x()){
            x = f.x();
            y = f.y();
        }else{
            x = s.x();
            y = s.y();
            std::swap(f, s);
        }

        int sgn = 1;
        if (f.y() > s.y())
        {
            sgn = -1;
        }

        while(x < s.x()){
            QPointF p;
            p.setX(roundf(x));
            p.setY(roundf(y));
            QRect rect(QPoint(p.x() * 30, -p.y() * 30), QPoint(p.x() * 30 + 30, -p.y() * 30 - 30 * sgn));
            scene->addRect(rect, QPen(Qt::gray), QBrush(Qt::blue));
            x++;
            y = y + a;
        }
    }
    else
    {
        double a = dx/dy;
        double x,y;
        if(f.y()<=s.y()){
            x = f.x();
            y = f.y();
        }
        else
        {
            x = s.x();
            y = s.y();
            std::swap(f, s);
        }

        int sgn = 1;
        if (f.x() > s.x())
        {
            sgn = -1;
        }

        while(y < s.y()){
            QPointF p;
            p.setX(roundf(x));
            p.setY(roundf(y));
            //qDebug()<<p << '\n';
            QRect rect(QPoint(p.x() * 30, -p.y() * 30), QPoint(p.x() * 30 + 30 * sgn, -p.y() * 30 - 30));
            scene->addRect(rect, QPen(Qt::gray), QBrush(Qt::blue));
            y++;
            x = x + a;
        }
    }
}

void Plane::dda(QPoint f, QPoint s)
{
    int l = std::max(abs(s.x() - f.x()), abs(s.y() - f.y()));
    double x = f.x();
    double y = f.y();
    int sgnx = 1;
    int sgny = 1;
    if (f.x() > s.x())
    {
        sgnx = -1;
    }

    if (f.y() > s.y())
    {
        sgny = -1;
    }

    const int L = l;
    while (l > 0)
    {
        QPointF p;
        p.setX(roundf(x));
        p.setY(roundf(y));
        //qDebug()<<p << '\n';
        QRect rect(QPoint(p.x() * 30, -p.y() * 30), QPoint(p.x() * 30 + 30 * sgnx, -p.y() * 30 - 30 * sgny));
        scene->addRect(rect, QPen(Qt::gray), QBrush(Qt::blue));
        x+=(double)(s.x() - f.x()) / (double)L;
        y+=(double)(s.y() - f.y()) / (double)L;
        l--;
    }

    scene->addEllipse(f.x() * 30 - 5, -f.y() * 30 - 5, 10, 10, QPen(Qt::green));
    scene->addEllipse(s.x() * 30 - 5, -s.y() * 30 - 5, 10, 10, QPen(Qt::green));
}

void Plane::bresenham(QPoint f, QPoint s)
{
    if (f.x() > s.x())
    {
        std::swap(f, s);
    }

    int spaceTransferX = f.x();
    int spaceTransferY = f.y();
    int x = 0; int y = 0;
    int endX = s.x() - spaceTransferX;
    int endY = s.y() - spaceTransferY;
    int sgnI = (endY >= 0) - (endY < 0);
    bool swapped = false;
    if (abs(endY) > endX)
    {
        endY = abs(endY);
        std::swap(endX, endY);
        std::swap(spaceTransferX, spaceTransferY);
        swapped = true;
    }
    else
    {
        endY = abs(endY);
    }

    double dprev = endX;
    for(int cntIterations = endX; cntIterations > 0; cntIterations--)
    {
        QPointF p;
        p.setX(roundf(x));
        p.setY(roundf(y));

        if (!swapped)
        {
            QRect rect(QPoint((spaceTransferX + p.x()) * 30, -(spaceTransferY + sgnI * p.y()) * 30),
                       QPoint((spaceTransferX + p.x()) * 30 + 30,-(spaceTransferY + sgnI * p.y()) * 30 - 30 * sgnI));
            scene->addRect(rect, QPen(Qt::gray), QBrush(Qt::blue));
        }
        else
        {
            QRect rect(QPoint((spaceTransferY + p.y()) * 30, -(spaceTransferX + sgnI * p.x()) * 30),
                       QPoint((spaceTransferY + p.y()) * 30 + 30,-(spaceTransferX + sgnI * p.x()) * 30 - 30 * sgnI));
            scene->addRect(rect, QPen(Qt::gray), QBrush(Qt::blue));
        }

        double d = dprev + 2 * endY;
        if (dprev >= 0)
        {
            d -= 2 * endX;
        }

        int dy = d >= 0;
        x++;
        y+=dy;
        dprev = d;
        //qDebug() << x << ' ' << y <<'\n';
    }


    scene->addEllipse(f.x() * 30 - 5, -f.y() * 30 - 5, 10, 10, QPen(Qt::green));
    scene->addEllipse(s.x() * 30 - 5, -s.y() * 30 - 5, 10, 10, QPen(Qt::green));
}

void Plane::circle(QPoint f, int radius)
{
    int x = 0, y = radius, delta = (3 - 2 * radius);
    while (x <= y)
    {
        scene->addRect(QRect(QPoint((f.x() + x) * 30, -(f.y() + y) * 30), QPoint((f.x() + x) * 30 + 30, -(f.y() + y) * 30 - 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addRect(QRect(QPoint((f.x() + x) * 30, -(f.y() - y) * 30), QPoint((f.x() + x) * 30 + 30, -(f.y() - y) * 30 + 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addRect(QRect(QPoint((f.x() - x) * 30, -(f.y() + y) * 30), QPoint((f.x() - x) * 30 - 30, -(f.y() + y) * 30 - 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addRect(QRect(QPoint((f.x() - x) * 30, -(f.y() - y) * 30), QPoint((f.x() - x) * 30 - 30, -(f.y() - y) * 30 + 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addEllipse((f.x() + x) * 30 - 5, -(f.y() + y) * 30 - 5, 10, 10, QPen(Qt::green));
        scene->addEllipse((f.x() + x) * 30 - 5, -(f.y() - y) * 30 - 5, 10, 10, QPen(Qt::green));
        scene->addEllipse((f.x() - x) * 30 - 5, -(f.y() + y) * 30 - 5, 10, 10, QPen(Qt::green));
        scene->addEllipse((f.x() - x) * 30 - 5, -(f.y() - y) * 30 - 5, 10, 10, QPen(Qt::green));

        scene->addRect(QRect(QPoint((f.x() + y) * 30, -(f.y() + x) * 30), QPoint((f.x() + y) * 30 + 30, -(f.y() + x) * 30 - 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addRect(QRect(QPoint((f.x() + y) * 30, -(f.y() - x) * 30), QPoint((f.x() + y) * 30 + 30, -(f.y() - x) * 30 + 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addRect(QRect(QPoint((f.x() - y) * 30, -(f.y() + x) * 30), QPoint((f.x() - y) * 30 - 30, -(f.y() + x) * 30 - 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addRect(QRect(QPoint((f.x() - y) * 30, -(f.y() - x) * 30), QPoint((f.x() - y) * 30 - 30, -(f.y() - x) * 30 + 30)), QPen(Qt::gray), QBrush(Qt::red));
        scene->addEllipse((f.x() + y) * 30 - 5, -(f.y() + x) * 30 - 5, 10, 10, QPen(Qt::green));
        scene->addEllipse((f.x() + y) * 30 - 5, -(f.y() - x) * 30 - 5, 10, 10, QPen(Qt::green));
        scene->addEllipse((f.x() - y) * 30 - 5, -(f.y() + x) * 30 - 5, 10, 10, QPen(Qt::green));
        scene->addEllipse((f.x() - y) * 30 - 5, -(f.y() - x) * 30 - 5, 10, 10, QPen(Qt::green));


        delta += delta < 0 ? 4 * x + 6 : 4 * (x - y--) + 10;
        ++x;
    }
}

