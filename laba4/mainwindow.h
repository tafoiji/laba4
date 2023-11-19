#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QScreen>
#include <plane.h>
#include <QDebug>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSize screenSize;
private:
    QPushButton* iterationsBtn;
    QPushButton* ddaBtn;
    QPushButton* bresenhamBtn;
    QPushButton* circleBtn;
    QSpinBox* x1;
    QSpinBox* y1;
    QSpinBox* x2;
    QSpinBox* y2;
    QSpinBox* r;

    Ui::MainWindow *ui;

public slots:
    void iterations();
    void dda();
    void bresenham();
    void circle();
    void changeRangeOfRadius(int);
};
#endif // MAINWINDOW_H
