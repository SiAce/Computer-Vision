#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <QtGui>
#include <QtCore>

namespace Ui {
class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = 0);
    Histogram(long * array);
    ~Histogram();

private:
    Ui::Histogram *ui;

protected:
    void paintEvent(QPaintEvent *event);
    long * his;
};

#endif // HISTOGRAM_H
