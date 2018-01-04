#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
}

Histogram::Histogram(long *array) :
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
    his = array;
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen linepen(Qt::red);
    linepen.setWidth(4);
    painter.setPen(linepen);
    QPoint p1,p2;

    for(int i = 0; i < 256; i++)
    {
        int value = his[i];
        p1.setX(20 + 4*i);
        p1.setY(700 - value);
        p2.setX(20 + 4*i);
        p2.setY(700);
        painter.drawLine(p1,p2);
    }
}
