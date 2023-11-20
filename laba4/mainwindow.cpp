#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedHeight(440);

    QVBoxLayout* layout = new QVBoxLayout(this->centralWidget());
    x1 = new QSpinBox(this->centralWidget());
    y1 = new QSpinBox(this->centralWidget());
    x2 = new QSpinBox(this->centralWidget());
    y2 = new QSpinBox(this->centralWidget());
    r = new QSpinBox(this->centralWidget());
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  scr = screen->geometry();
    screenSize = QSize(scr.width() / 30 * 30, scr.height() / 30 * 30);
    connect(x1, SIGNAL(valueChanged(int)), this, SLOT(changeRangeOfRadius(int)));
    connect(y1, SIGNAL(valueChanged(int)), this, SLOT(changeRangeOfRadius(int)));
    x1->setRange(-screenSize.width() / 60  + 1, screenSize.width() / 60 - 1);
    y1->setRange(-screenSize.height() / 60 + 1, screenSize.height() / 60 - 1);
    x2->setRange(-screenSize.width() / 60 + 1, screenSize.width() / 60 - 1);
    y2->setRange(-screenSize.height() / 60 + 1, screenSize.height() / 60 - 1);
    changeRangeOfRadius(0);

    QLabel *label = new QLabel("choose first point", this->centralWidget());
    QLabel *label2 = new QLabel("choose second point", this->centralWidget());
    QLabel *labRad = new QLabel("Choose circle radius (for bresenham circle method only)", this->centralWidget());
    iterationsBtn = new QPushButton("Iterations Method", this->centralWidget());
    ddaBtn = new QPushButton("DDA Line Method", this->centralWidget());
    bresenhamBtn = new QPushButton("Bresenham Method", this->centralWidget());
    circleBtn = new QPushButton("Bresenham Method For Circle", this->centralWidget());

    connect(iterationsBtn, SIGNAL(pressed()), this, SLOT(iterations()));
    connect(ddaBtn, SIGNAL(pressed()), this, SLOT(dda()));
    connect(bresenhamBtn, SIGNAL(pressed()), this, SLOT(bresenham()));
    connect(circleBtn, SIGNAL(pressed()), this, SLOT(circle()));

    layout->addWidget(label);
    layout->addWidget(x1);
    layout->addWidget(y1);
    layout->addWidget(label2);
    layout->addWidget(x2);
    layout->addWidget(y2);
    layout->addWidget(labRad);
    layout->addWidget(r);
    layout->addWidget(iterationsBtn);
    layout->addWidget(ddaBtn);
    layout->addWidget(bresenhamBtn);
    layout->addWidget(circleBtn);

    //QWidget* widget = new QWidget();
    //widget->setLayout(layout);
    //this->setCentralWidget(widget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iterations()
{
    Plane* plane = new Plane(screenSize);
    auto start = std::chrono::steady_clock::now();
    plane->iterations(QPoint(x1->value(), y1->value()), QPoint(x2->value(), y2->value()));
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    QTextStream out(stdout);
    out << "time by iterations method: " << duration.count() << "ns\n";
    plane->show();
}

void MainWindow::dda()
{
    Plane* plane = new Plane(screenSize);
    auto start = std::chrono::steady_clock::now();
    plane->dda(QPoint(x1->value(), y1->value()), QPoint(x2->value(), y2->value()));
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    QTextStream out(stdout);
    out << "time by dda line method: " << duration.count() << "ns\n";
    plane->show();
}

void MainWindow::bresenham()
{
    Plane* plane = new Plane(screenSize);

    auto start = std::chrono::steady_clock::now();
    plane->bresenham(QPoint(x1->value(), y1->value()), QPoint(x2->value(), y2->value()));
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    QTextStream out(stdout);
    out << "time by bresenham method: " << duration.count() << "ns\n";
    plane->show();
}

void MainWindow::circle()
{
    Plane* plane = new Plane(screenSize);

    auto start = std::chrono::steady_clock::now();
    plane->circle(QPoint(x1->value(), y1->value()), r->value());
    auto stop = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    QTextStream out(stdout);
    out << "time by bresenham circle method: " << duration.count() << "ns\n";
    plane->show();
}

void MainWindow::changeRangeOfRadius(int)
{
    r->setRange(0, std::min(std::min(x1->maximum() - x1->value(), x1->value() - x1->minimum()), std::min(y1->maximum() - y1->value(), y1->value() - y1->minimum())));
}
