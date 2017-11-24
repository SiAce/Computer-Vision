#include "notepad.h"
#include "ui_notepad.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::on_actionNew_triggered()
{
    currentFile = "";
       ui->textEdit->setText("");
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
        QFile file(fileName);
        currentFile = fileName;
        if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this,"..","File not opened.");
            return;
        }
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();
}

void Notepad::on_actionSave_triggered()
{
    QFile file(currentFile);
        if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this,"..","No file opened. Use Save As");
            return;
        }
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.flush();
        file.close();
}

void Notepad::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
        QFile file(fileName);
        currentFile = fileName;
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this,"..","File not opened.");
            return;
        }
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.flush();
        file.close();
}
