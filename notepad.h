#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QLabel>
#include <QScrollArea>
#include <cmath>
#include <algorithm>
#include <array>
#define PI 3.14159265

namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

public slots:
    void changeMin(int value);
    void changeMax(int value);

private slots:

    void on_actionOpen_triggered();

    void on_actionGrey_triggered();

    void on_actionRed_triggered();

    void on_actionGreen_triggered();

    void on_actionBlue_triggered();

    void on_actionHue_triggered();

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void on_actionSaturation_triggered();

    void on_actionBrightness_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOtsu_triggered();

    void on_actionDouble_Threshold_triggered();

    void on_actionCrop_triggered();

    void on_actionOpen_Second_triggered();

    void on_actionAdd_triggered();

    void on_actionSubtract_triggered();

    void on_actionMultiply_triggered();

    void on_actionNearest_triggered();

    void on_actionBilinear_triggered();

    void on_actionNearest_2_triggered();

    void on_actionBilinear_2_triggered();

    void on_actionHistogram_triggered();

    void on_actionNormal_Linear_triggered();

    void on_actionPiecewise_Linear_triggered();

    void on_actionLogarithm_triggered();

    void on_actionExponent_triggered();

    void on_actionEqualization_triggered();

    void on_actionMean_triggered();

    void on_actionMedian_triggered();

    void on_actionGauss_triggered();

    void on_actionCustom_Kernel_triggered();

    void on_actionSobel_triggered();

    void on_actionLaplace_triggered();

    void on_actionCanny_triggered();

private:
    Ui::Notepad *ui;
    QString currentFile;
    QImage *image;
    QImage *image_2;
    QImage *processed_image;
    QLabel *imageLabel;
    QLabel *imageLabel_2;
    QStatusBar* m_pStatus;

    int min = 0, max = 0;
    QPoint mouse_begin, mouse_end;

};

#endif // NOTEPAD_H
