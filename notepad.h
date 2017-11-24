#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QString>

namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

private:
    Ui::Notepad *ui;
    QString currentFile;
};

#endif // NOTEPAD_H
