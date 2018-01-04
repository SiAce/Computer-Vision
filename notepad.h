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
