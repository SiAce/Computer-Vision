#include "notepad.h"
#include "ui_notepad.h"
#include "histogram.h"

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{   
    ui->setupUi(this);

    setMouseTracking(true);

    m_pStatus = new QStatusBar();
    setStatusBar(m_pStatus);

    image = new QImage;
    image_2 = new QImage;
    processed_image = new QImage;

    imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignTop);
    imageLabel->setMouseTracking(true);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(imageLabel);

    imageLabel_2 = new QLabel;
    imageLabel_2->setAlignment(Qt::AlignTop);
    ui->scrollArea_2->setBackgroundRole(QPalette::Dark);
    ui->scrollArea_2->setWidget(imageLabel_2);

    ui->threshold1->setMinimum(0);
    ui->threshold1->setMaximum(255);
    ui->threshold2->setMinimum(0);
    ui->threshold2->setMaximum(255);
    ui->threshold1->setTracking(true);
    ui->threshold2->setTracking(true);

    m_pStatus->showMessage("application init ok!");

}

Notepad::~Notepad()
{
    delete ui;
}

QImage Notepad::Subtract(QImage input_image_1, QImage input_image_2)
{
    int width = (input_image_1.width()<input_image_2.width())?input_image_1.width():input_image_2.width();
    int height = (input_image_1.height()<input_image_2.height())?input_image_1.height():input_image_2.height();
    QImage new_image(width, height, QImage::Format_ARGB32);

    QRgb rgb, rgb_2;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            rgb = input_image_1.pixel(i, j);
            rgb_2 = input_image_2.pixel(i,j);
            int red = (qRed(rgb) - qRed(rgb_2) > 0)? (qRed(rgb) - qRed(rgb_2)): 0;
            int green = (qGreen(rgb) - qGreen(rgb_2) > 0)? (qGreen(rgb) - qGreen(rgb_2)): 0;
            int blue = (qBlue(rgb) - qBlue(rgb_2) > 0)? (qBlue(rgb) - qBlue(rgb_2)): 0;
            new_image.setPixel(i, j, qRgb(red, green, blue));
        }
    }

    return new_image;
}

QImage Notepad::Dilation(QImage input_image)
{
    int width = input_image.width();
    int height = input_image.height();
    QImage new_image(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            new_image.setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            if(qRed(input_image.pixel(i, j)) == 255)
            {
                for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
                {
                    for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                    {
                        if(structure_element[origin - 1 + x + 3*y] == 1)
                        {
                            new_image.setPixel(i + x, j + y, qRgb(255, 255, 255));
                        }
                    }
                }
            }
        }
    }

    return new_image;
}

QImage Notepad::Erosion(QImage input_image)
{
    int width = input_image.width();
    int height = input_image.height();
    QImage new_image(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            new_image.setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int flag = 1;

            for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
            {
                for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                {
                    if((structure_element[origin - 1 + x + 3*y] == 1) && (qRed(input_image.pixel(i + x, j + y)) != 255))
                    {
                        flag = 0;
                    }
                }
            }

            if (flag == 1)
            {
                new_image.setPixel(i , j , qRgb(255, 255, 255));
            }
        }
    }

    return new_image;
}

QImage Notepad::Opening(QImage input_image)
{
    return Dilation(Erosion(input_image));
}

QImage Notepad::Complement(QImage input_image)
{
    int width = input_image.width();
    int height = input_image.height();
    QImage new_image(width, height, QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<height; y++){
        line = (QRgb *)input_image.scanLine(y);

        for(int x = 0; x<width; x++){
            int gray = 255 - qRed(line[x]);
            new_image.setPixel(x,y, qRgb(gray, gray, gray));
        }
    }

    return new_image;
}

QImage Notepad::Intersection(QImage input_image_1, QImage input_image_2)
{
    int width = input_image_1.width();
    int height = input_image_1.height();
    QImage new_image(width, height, QImage::Format_ARGB32);

    QRgb * line_1, * line_2;

    for(int y = 0; y<height; y++){
        line_1 = (QRgb *)input_image_1.scanLine(y);
        line_2 = (QRgb *)input_image_2.scanLine(y);

        for(int x = 0; x<width; x++){
            if (qRed(line_1[x]) == 255 && qRed(line_2[x]) == 255)
            {
                new_image.setPixel(x,y, qRgb(255, 255, 255));
            }else
            {
                new_image.setPixel(x,y, qRgb(0, 0, 0));
            }

        }
    }

    return new_image;
}

QImage Notepad::Union(QImage input_image_1, QImage input_image_2)
{
    int width = input_image_1.width();
    int height = input_image_1.height();
    QImage new_image(width, height, QImage::Format_ARGB32);

    QRgb * line_1, * line_2;

    for(int y = 0; y<height; y++){
        line_1 = (QRgb *)input_image_1.scanLine(y);
        line_2 = (QRgb *)input_image_2.scanLine(y);

        for(int x = 0; x<width; x++){
            if (qRed(line_1[x]) == 255 || qRed(line_2[x]) == 255)
            {
                new_image.setPixel(x,y, qRgb(255, 255, 255));
            }else
            {
                new_image.setPixel(x,y, qRgb(0, 0, 0));
            }

        }
    }

    return new_image;
}

QImage Notepad::HitOrMiss(QImage input_image)
{
    int width = input_image.width();
    int height = input_image.height();
    QImage new_image(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    if(ui->structuringElement11->toPlainText().isEmpty())
    {
        structure_element[0] = -1;
    }else
    {
        structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    }

    if(ui->structuringElement12->toPlainText().isEmpty())
    {
        structure_element[1] = -1;
    }else
    {
        structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    }

    if(ui->structuringElement13->toPlainText().isEmpty())
    {
        structure_element[2] = -1;
    }else
    {
        structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    }

    if(ui->structuringElement21->toPlainText().isEmpty())
    {
        structure_element[3] = -1;
    }else
    {
        structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    }

    if(ui->structuringElement22->toPlainText().isEmpty())
    {
        structure_element[4] = -1;
    }else
    {
        structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    }

    if(ui->structuringElement23->toPlainText().isEmpty())
    {
        structure_element[5] = -1;
    }else
    {
        structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    }

    if(ui->structuringElement31->toPlainText().isEmpty())
    {
        structure_element[6] = -1;
    }else
    {
        structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    }

    if(ui->structuringElement32->toPlainText().isEmpty())
    {
        structure_element[7] = -1;
    }else
    {
        structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    }

    if(ui->structuringElement33->toPlainText().isEmpty())
    {
        structure_element[8] = -1;
    }else
    {
        structure_element[8] = ui->structuringElement33->toPlainText().toInt();
    }

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int flag = 1;

            for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
            {
                for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                {
                    if((structure_element[origin - 1 + x + 3*y] != -1) && (structure_element[origin - 1 + x + 3*y] != (qRed(input_image.pixel(i + x, j + y)) / 255)))
                    {
                        flag = 0;
                    }
                }
            }

            if (flag == 1)
            {
                new_image.setPixel(i , j , qRgb(255, 255, 255));
            } else
            {
                new_image.setPixel(i , j , qRgb(0, 0, 0));
            }
        }
    }

    return new_image;
}

QImage Notepad::GrayDilation(QImage input_image)
{
    int width = input_image.width();
    int height = input_image.height();
    QImage new_image(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            new_image.setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int max = 0;

            for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
            {
                for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                {
                    if(structure_element[origin - 1 + x + 3*y] == 1 && qRed(input_image.pixel(i + x, j + y)) > max)
                    {
                        max = qRed(input_image.pixel(i + x, j + y));
                    }
                }
            }

            new_image.setPixel(i, j, qRgb(max, max, max));
        }
    }

    return new_image;
}

void Notepad::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = imageLabel->mapFromGlobal(QCursor::pos());
    QRgb rgb = image->pixel(p.x(),p.y());
    m_pStatus->showMessage(QString("x: %1 y: %2    RGB(%3, %4, %5)").arg(p.x()).arg(p.y())
                           .arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb)));
}

void Notepad::mousePressEvent(QMouseEvent *event)
{
    mouse_begin = imageLabel->mapFromGlobal(QCursor::pos());
}

void Notepad::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_end = imageLabel->mapFromGlobal(QCursor::pos());
}

void Notepad::changeMin(int value)
{
    min = value;

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            if (average >= min && average < max)
            {
                newImage->setPixel(x,y, qRgb(255, 255, 255));
            } else {
                newImage->setPixel(x,y, qRgb(0, 0, 0));
            }

        }

    }
    *processed_image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*processed_image));
    delete newImage;
}

void Notepad::changeMax(int value)
{
    max = value;

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            if (average >= min && average < max)
            {
                newImage->setPixel(x,y, qRgb(255, 255, 255));
            } else {
                newImage->setPixel(x,y, qRgb(0, 0, 0));
            }

        }

    }
    *processed_image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*processed_image));
    delete newImage;
}

void Notepad::changeLevelMin(int value)
{
    min = value;
    int a = min;
    int b = max;
    int c = 0;
    int d = 255;

    int size = 256;

    int red_min = 0, red_max = 255, green_min = 0, green_max = 255, blue_min = 0, blue_max= 255;

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red0 = qRed(line[x]), green0 = qGreen(line[x]), blue0 = qBlue(line[x]);
            int red, green, blue;

            if(red0 >= red_min && red0 < a)
            {
                red = c*(red0 - red_min)/(a - red_min);
            } else if (red0 >= a && red0 < b) {
                red = (d - c)*(red0 - a)/(b - a) + c;
            } else {
                red = (255 - d)*(red0 - b)/(red_max - b) + d;
            }

            if(green0 >= green_min && green0 < a)
            {
                green = c*(green0 - green_min)/(a - green_min);
            } else if (green0 >= a && green0 < b) {
                green = (d - c)*(green0 - a)/(b - a) + c;
            } else {
                green = (255 - d)*(green0 - b)/(green_max - b) + d;
            }

            if(blue0 >= blue_min && blue0 < a)
            {
                blue = c*(blue0 - blue_min)/(a - blue_min);
            } else if (blue0 >= a && blue0 < b) {
                blue = (d - c)*(blue0 - a)/(b - a) + c;
            } else {
                blue = (255 - d)*(blue0 - b)/(blue_max - b) + d;
            }

            newImage->setPixel(x,y, qRgb(red, green, blue));
        }

    }

    *processed_image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*processed_image));
    delete newImage;
}

void Notepad::changeLevelMax(int value)
{
    max = value;
    int a = min;
    int b = max;
    int c = 0;
    int d = 255;

    int size = 256;

    int red_min = 0, red_max = 255, green_min = 0, green_max = 255, blue_min = 0, blue_max= 255;

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red0 = qRed(line[x]), green0 = qGreen(line[x]), blue0 = qBlue(line[x]);
            int red, green, blue;

            if(red0 >= red_min && red0 < a)
            {
                red = c*(red0 - red_min)/(a - red_min);
            } else if (red0 >= a && red0 < b) {
                red = (d - c)*(red0 - a)/(b - a) + c;
            } else {
                red = (255 - d)*(red0 - b)/(red_max - b) + d;
            }

            if(green0 >= green_min && green0 < a)
            {
                green = c*(green0 - green_min)/(a - green_min);
            } else if (green0 >= a && green0 < b) {
                green = (d - c)*(green0 - a)/(b - a) + c;
            } else {
                green = (255 - d)*(green0 - b)/(green_max - b) + d;
            }

            if(blue0 >= blue_min && blue0 < a)
            {
                blue = c*(blue0 - blue_min)/(a - blue_min);
            } else if (blue0 >= a && blue0 < b) {
                blue = (d - c)*(blue0 - a)/(b - a) + c;
            } else {
                blue = (255 - d)*(blue0 - b)/(blue_max - b) + d;
            }

            newImage->setPixel(x,y, qRgb(red, green, blue));
        }

    }

    *processed_image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*processed_image));
    delete newImage;
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    QImage new_image(fileName);
    *image = new_image;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionSave_triggered()
{
    image->save(currentFile);
}

void Notepad::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save the file");
    image->save(fileName);
}

void Notepad::on_actionOpen_Second_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the second file");
    QImage new_image(fileName);
    *image_2 = new_image;
    imageLabel_2->setPixmap(QPixmap::fromImage(*image_2));
}

void Notepad::on_actionRed_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            newImage->setPixel(x,y, qRgb(qRed(line[x]), 0, 0));
        }

    }
    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionGreen_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            newImage->setPixel(x,y, qRgb(0, qGreen(line[x]), 0));
        }

    }
    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionBlue_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            newImage->setPixel(x,y, qRgb(0, 0, qBlue(line[x])));
        }

    }
    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionGrey_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            newImage->setPixel(x,y, qRgb(average, average, average));
        }

    }
    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionHue_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);
    int value = ui->hueText->toPlainText().toInt();
    for(int i = 0; i<image->width(); i++)
    {
        for(int j = 0; j<image->height(); j++)
        {
            QColor color = image->pixelColor(i,j);
            int hue = color.hue();
            hue = hue + value;
            color.setHsv(hue, color.saturation(), color.value(), color.alpha());
            newImage->setPixelColor(i, j, color);
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionSaturation_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);
    int value = ui->hueText->toPlainText().toInt();
    for(int i = 0; i<image->width(); i++)
    {
        for(int j = 0; j<image->height(); j++)
        {
            QColor color = image->pixelColor(i,j);
            int saturation = color.saturation();
            if(saturation + value > 255){
                saturation = 255;
            }else if(saturation + value < 0){
                saturation = 0;
            }else {
                saturation = saturation + value;
            }
            color.setHsv(color.hue(), saturation, color.value(), color.alpha());
            newImage->setPixelColor(i, j, color);
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionBrightness_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);
    int increasement = ui->hueText->toPlainText().toInt();
    for(int i = 0; i<image->width(); i++)
    {
        for(int j = 0; j<image->height(); j++)
        {
            QColor color = image->pixelColor(i,j);
            int value = color.value();
            if(value + increasement > 255){
                value = 255;
            }else if(value + increasement < 0){
                value = 0;
            }else{
                value = value + increasement;
            }
            color.setHsv(color.hue(), color.saturation(), value, color.alpha());
            newImage->setPixelColor(i, j, color);
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionLevel_triggered()
{
    QObject::disconnect(ui->threshold1, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeMin(int)));
    QObject::disconnect(ui->threshold2, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeMax(int)));

    QObject::connect(ui->threshold1, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeLevelMin(int)));
    QObject::connect(ui->threshold2, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeLevelMax(int)));
}

void Notepad::on_actionOtsu_triggered()
{
    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;
    int total_pixels = 0;
    int greylevel[256] = {0};
    double probabilities[256] = {0};
    double w0 = 0, w1 = 0, u0 = 0, u1 = 0, last_sigma2 = 0,sigma2 = 0;
    int t = 0, final_t = 0;

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            total_pixels++;
            greylevel[average]++;
        }

    }

    for(int x = 0; x<256; x++)
    {
        probabilities[x] = double(greylevel[x]) / total_pixels;
    }

    for(t = 1; t<256; t++)
    {
        w0 = 0, w1 = 0, u0 = 0, u1 = 0;
        for(int x = 0; x<t; x++)
        {
            w0 = w0 + probabilities[x];
        }
        for(int x = t; x<256; x++)
        {
            w1 = w1 + probabilities[x];
        }
        for(int x = 0; x<t; x++)
        {
            u0 = u0 + x*probabilities[x]/w0;
        }
        for(int x = t; x<256; x++)
        {
            u1 = u1 + x*probabilities[x]/w1;
        }
        sigma2 = w0*w1*((u0 - u1)*(u0 - u1));
        if (sigma2 > last_sigma2)
        {
            final_t = t;
        }
        last_sigma2 = sigma2;
    }

    for(int y = 0; y<newImage->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<newImage->width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            if (average >= final_t)
            {
                newImage->setPixel(x,y, qRgb(255, 255, 255));
            } else {
                newImage->setPixel(x,y, qRgb(0, 0, 0));
            }

        }

    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionDouble_Threshold_triggered()
{
    QObject::disconnect(ui->threshold1, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeLevelMin(int)));
    QObject::disconnect(ui->threshold2, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeLevelMax(int)));

    QObject::connect(ui->threshold1, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeMin(int)));
    QObject::connect(ui->threshold2, SIGNAL(sliderMoved(int)),
                     this, SLOT(changeMax(int)));
}

void Notepad::on_actionCrop_triggered()
{
    QRect rect(mouse_begin, mouse_end);
    QImage cropped = image->copy(rect);
    *image = cropped;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionAdd_triggered()
{
    int width = (image->width()<image_2->width())?image->width():image_2->width();
    int height = (image->height()<image_2->height())?image->height():image_2->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb, rgb_2;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            rgb = image->pixel(i, j);
            rgb_2 = image_2->pixel(i,j);
            int red = (qRed(rgb) + qRed(rgb_2) < 256)? (qRed(rgb) + qRed(rgb_2)): 255;
            int green = (qGreen(rgb) + qGreen(rgb_2) < 256)? (qGreen(rgb) + qGreen(rgb_2)): 255;
            int blue = (qBlue(rgb) + qBlue(rgb_2) < 256)? (qBlue(rgb) + qBlue(rgb_2)): 255;
            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionSubtract_triggered()
{
    int width = (image->width()<image_2->width())?image->width():image_2->width();
    int height = (image->height()<image_2->height())?image->height():image_2->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb, rgb_2;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            rgb = image->pixel(i, j);
            rgb_2 = image_2->pixel(i,j);
            int red = (qRed(rgb) - qRed(rgb_2) > 0)? (qRed(rgb) - qRed(rgb_2)): 0;
            int green = (qGreen(rgb) - qGreen(rgb_2) > 0)? (qGreen(rgb) - qGreen(rgb_2)): 0;
            int blue = (qBlue(rgb) - qBlue(rgb_2) > 0)? (qBlue(rgb) - qBlue(rgb_2)): 0;
            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionMultiply_triggered()
{
    int width = (image->width()<image_2->width())?image->width():image_2->width();
    int height = (image->height()<image_2->height())?image->height():image_2->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb, rgb_2;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            rgb = image->pixel(i, j);
            rgb_2 = image_2->pixel(i,j);
            int red = (qRed(rgb) * qRed(rgb_2) < 256)? (qRed(rgb) * qRed(rgb_2)): 255;
            int green = (qGreen(rgb) * qGreen(rgb_2) < 256)? (qGreen(rgb) * qGreen(rgb_2)): 255;
            int blue = (qBlue(rgb) * qBlue(rgb_2) < 256)? (qBlue(rgb) * qBlue(rgb_2)): 255;
            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionNearest_triggered()
{
    double value = ui->scaleText->toPlainText().toDouble();
    int width = image->width()*value;
    int height = image->height()*value;
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);
    QRgb rgb;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            rgb = image->pixel(int(i/value), int(j/value));
            newImage->setPixel(i, j, rgb);
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionBilinear_triggered()
{
    double value = ui->scaleText->toPlainText().toDouble();
    int width = image->width()*value;
    int height = image->height()*value;
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);
    QRgb rgb11, rgb21, rgb12, rgb22;

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int x0 = i/value;
            int y0 = j/value;
            rgb11 = image->pixel(x0, y0);
            rgb21 = image->pixel(x0+1, y0);
            rgb12 = image->pixel(x0, y0+1);
            rgb22 = image->pixel(x0+1, y0+1);
            int x1 = x0*value;
            int y1 = y0*value;
            int x2 = (x0 + 1)*value;
            int y2 = (y0 + 1)*value;
            int a = (x2 - x1)*(y2 - y1);
            int red = double(qRed(rgb11)*(x2 - i)*(y2 - j) +
                    qRed(rgb21)*(i - x1)*(y2 - j) +
                    qRed(rgb12)*(x2 - i)*(j - y1) +
                    qRed(rgb22)*(i - x1)*(j - y1)) / a;
            int green = double(qGreen(rgb11)*(x2 - i)*(y2 - j) +
                    qGreen(rgb21)*(i - x1)*(y2 - j) +
                    qGreen(rgb12)*(x2 - i)*(j - y1)  +
                    qGreen(rgb22)*(i - x1)*(j - y1)) / a;
            int blue = double(qBlue(rgb11)*(x2 - i)*(y2 - j) +
                    qBlue(rgb21)*(i - x1)*(y2 - j) +
                    qBlue(rgb12)*(x2 - i)*(j - y1) +
                    qBlue(rgb22)*(i - x1)*(j - y1)) / a;
            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionNearest_2_triggered()
{
    double value = ui->rotateAngleText->toPlainText().toDouble();
    QMatrix rm;
    rm.rotate(value);
    *image = image->transformed(rm);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionBilinear_2_triggered()
{
    double value = ui->rotateAngleText->toPlainText().toDouble();
    QMatrix rm;
    rm.rotate(value);
    *image = image->transformed(rm);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionNormal_Linear_triggered()
{
    int size = 256;
    int red_array[256] = {0};
    int green_array[256] = {0};
    int blue_array[256] = {0};

    int red_min = 0, red_max = 255, green_min = 0, green_max = 255, blue_min = 0, blue_max= 255;

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            red_array[qRed(line[x])] = 1;
            green_array[qGreen(line[x])] = 1;
            blue_array[qBlue(line[x])] = 1;
        }

    }

    for(int x = 0; x < size; x++)
    {
        if(red_array[x] >= 1)
        {
            red_max = x;
        }

        if(red_array[255 - x] >= 1)
        {
            red_min = 255 - x;
        }

        if(green_array[x] >= 1)
        {
            green_max = x;
        }

        if(green_array[255 - x] >= 1)
        {
            green_min = 255 - x;
        }

        if(blue_array[x] >= 1)
        {
            blue_max = x;
        }

        if(blue_array[255 - x] >= 1)
        {
            blue_min = 255 - x;
        }
    }

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red = 255*(qRed(line[x]) - red_min)/(red_max - red_min);
            int green = 255*(qGreen(line[x]) - green_min)/(green_max - green_min);
            int blue = 255*(qBlue(line[x]) - blue_min)/(blue_max - blue_min);
            newImage->setPixel(x,y, qRgb(red, green, blue));
        }

    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionPiecewise_Linear_triggered()
{
    int a = ui->aText->toPlainText().toInt();
    int b = ui->bText->toPlainText().toInt();
    int c = ui->cText->toPlainText().toInt();
    int d = ui->dText->toPlainText().toInt();

    int size = 256;
    int red_array[256] = {0};
    int green_array[256] = {0};
    int blue_array[256] = {0};

    int red_min = 0, red_max = 255, green_min = 0, green_max = 255, blue_min = 0, blue_max= 255;

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            red_array[qRed(line[x])] = 1;
            green_array[qGreen(line[x])] = 1;
            blue_array[qBlue(line[x])] = 1;
        }

    }

    for(int x = 0; x < size; x++)
    {
        if(red_array[x] >= 1)
        {
            red_max = x;
        }

        if(red_array[255 - x] >= 1)
        {
            red_min = 255 - x;
        }

        if(green_array[x] >= 1)
        {
            green_max = x;
        }

        if(green_array[255 - x] >= 1)
        {
            green_min = 255 - x;
        }

        if(blue_array[x] >= 1)
        {
            blue_max = x;
        }

        if(blue_array[255 - x] >= 1)
        {
            blue_min = 255 - x;
        }
    }

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red0 = qRed(line[x]), green0 = qGreen(line[x]), blue0 = qBlue(line[x]);
            int red, green, blue;

            if(red0 >= red_min && red0 < a)
            {
                red = c*(red0 - red_min)/(a - red_min);
            } else if (red0 >= a && red0 < b) {
                red = (d - c)*(red0 - a)/(b - a) + c;
            } else {
                red = (255 - d)*(red0 - b)/(red_max - b) + d;
            }

            if(green0 >= green_min && green0 < a)
            {
                green = c*(green0 - green_min)/(a - green_min);
            } else if (green0 >= a && green0 < b) {
                green = (d - c)*(green0 - a)/(b - a) + c;
            } else {
                green = (255 - d)*(green0 - b)/(green_max - b) + d;
            }

            if(blue0 >= blue_min && blue0 < a)
            {
                blue = c*(blue0 - blue_min)/(a - blue_min);
            } else if (blue0 >= a && blue0 < b) {
                blue = (d - c)*(blue0 - a)/(b - a) + c;
            } else {
                blue = (255 - d)*(blue0 - b)/(blue_max - b) + d;
            }

            newImage->setPixel(x,y, qRgb(red, green, blue));
        }

    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionLogarithm_triggered()
{
    double a = ui->logarithmAText->toPlainText().toDouble();
    double b = ui->logarithmBText->toPlainText().toDouble();
    double c = ui->logarithmCText->toPlainText().toDouble();

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red0 = qRed(line[x]), green0 = qGreen(line[x]), blue0 = qBlue(line[x]);
            int red = a + log(red0 + 1) / b / log(c);
            int green = a + log(green0 + 1) / b / log(c);
            int blue = a + log(blue0 + 1) / b / log(c);
            newImage->setPixel(x,y, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionExponent_triggered()
{
    double a = ui->logarithmAText->toPlainText().toDouble();
    double b = ui->logarithmBText->toPlainText().toDouble();
    double c = ui->logarithmCText->toPlainText().toDouble();

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red0 = qRed(line[x]), green0 = qGreen(line[x]), blue0 = qBlue(line[x]);
            int red = pow(b, c*(red0 - a)) - 1;
            int green = pow(b, c*(green0 - a)) - 1;
            int blue = pow(b, c*(blue0 - a)) - 1;
            newImage->setPixel(x,y, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionHistogram_triggered()
{
    int size = 256;
    long his[256] = {0};

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
            his[average]++;
        }

    }

    for(int i = 0; i < size; i++)
    {
        his[i] = his[i] / 40;
    }
    Histogram histogram(his);
    histogram.setModal(true);
    histogram.exec();
}

void Notepad::on_actionEqualization_triggered()
{
    int size = 256;
    long red_array[256] = {0};
    long green_array[256] = {0};
    long blue_array[256] = {0};
    long n = image->width() * image->height();
    double red_cum_array[256] = {0};
    double green_cum_array[256] = {0};
    double blue_cum_array[256] = {0};

    QImage * newImage = new QImage(image->width(), image->height(), QImage::Format_ARGB32);

    QRgb * line;

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            red_array[qRed(line[x])]++;
            green_array[qGreen(line[x])]++;
            blue_array[qBlue(line[x])]++;
        }

    }

    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y <= x; y++)
        {
            red_cum_array[x] += red_array[y] ;
            green_cum_array[x] += green_array[y];
            blue_cum_array[x] += blue_array[y];
        }
    }

    for(int y = 0; y<image->height(); y++){
        line = (QRgb *)image->scanLine(y);

        for(int x = 0; x<image->width(); x++){
            int red = red_cum_array[qRed(line[x])] * (size - 1) / n;
            int green = green_cum_array[qGreen(line[x])] * (size - 1) / n;
            int blue = blue_cum_array[qBlue(line[x])] * (size - 1) / n;
            newImage->setPixel(x,y, qRgb(red, green, blue));
        }

    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionMean_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);
            int red = (qRed(rgb11) + qRed(rgb12) + qRed(rgb13) +
                       qRed(rgb21) + qRed(rgb22) + qRed(rgb23) +
                       qRed(rgb31) + qRed(rgb32) + qRed(rgb33)) / 9;
            int green = (qGreen(rgb11) + qGreen(rgb12) + qGreen(rgb13) +
                       qGreen(rgb21) + qGreen(rgb22) + qGreen(rgb23) +
                       qGreen(rgb31) + qGreen(rgb32) + qGreen(rgb33)) / 9;
            int blue = (qBlue(rgb11) + qBlue(rgb12) + qBlue(rgb13) +
                       qBlue(rgb21) + qBlue(rgb22) + qBlue(rgb23) +
                       qBlue(rgb31) + qBlue(rgb32) + qBlue(rgb33)) / 9;
            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionMedian_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            std::array<int, 9> red = {qRed(rgb11), qRed(rgb12), qRed(rgb13),
                                      qRed(rgb21), qRed(rgb22), qRed(rgb23),
                                      qRed(rgb31), qRed(rgb32), qRed(rgb33)};
            std::sort(red.begin(), red.end());

            std::array<int, 9> green = {qGreen(rgb11), qGreen(rgb12), qGreen(rgb13),
                                      qGreen(rgb21), qGreen(rgb22), qGreen(rgb23),
                                      qGreen(rgb31), qGreen(rgb32), qGreen(rgb33)};
            std::sort(green.begin(), green.end());

            std::array<int, 9> blue = {qBlue(rgb11), qBlue(rgb12), qBlue(rgb13),
                                      qBlue(rgb21), qBlue(rgb22), qBlue(rgb23),
                                      qBlue(rgb31), qBlue(rgb32), qBlue(rgb33)};
            std::sort(blue.begin(), blue.end());

            newImage->setPixel(i, j, qRgb(red[4], green[4], blue[4]));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionGauss_triggered()
{
    double sigma = ui->sigmaText->toPlainText().toDouble();
    double c1 = exp(-1/(sigma * sigma));
    double c2 = exp(-1/(2 * sigma * sigma));
    double total = 1 + 4*c1 + 4*c2;
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            int red = (c2*qRed(rgb11) + c1*qRed(rgb12) + c2*qRed(rgb13) +
                       c1*qRed(rgb21) + qRed(rgb22) + c1*qRed(rgb23) +
                       c2*qRed(rgb31) + c1*qRed(rgb32) + c2*qRed(rgb33)) / total;
            int green = (c2*qGreen(rgb11) + c1*qGreen(rgb12) + c2*qGreen(rgb13) +
                         c1*qGreen(rgb21) + qGreen(rgb22) + c1*qGreen(rgb23) +
                         c2*qGreen(rgb31) + c1*qGreen(rgb32) + c2*qGreen(rgb33)) / total;
            int blue = (c2*qBlue(rgb11) + c1*qBlue(rgb12) + c2*qBlue(rgb13) +
                        c1*qBlue(rgb21) + qBlue(rgb22) + c1*qBlue(rgb23) +
                        c2*qBlue(rgb31) + c1*qBlue(rgb32) + c2*qBlue(rgb33)) / total;

            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionCustom_Kernel_triggered()
{
    double c11 = ui->kernelText11->toPlainText().toDouble();
    double c12 = ui->kernelText12->toPlainText().toDouble();
    double c13 = ui->kernelText13->toPlainText().toDouble();
    double c21 = ui->kernelText21->toPlainText().toDouble();
    double c22 = ui->kernelText22->toPlainText().toDouble();
    double c23 = ui->kernelText23->toPlainText().toDouble();
    double c31 = ui->kernelText31->toPlainText().toDouble();
    double c32 = ui->kernelText32->toPlainText().toDouble();
    double c33 = ui->kernelText33->toPlainText().toDouble();

    double total = fabs(c11) + fabs(c12) + fabs(c13) + fabs(c21) + fabs(c22) + fabs(c23) + fabs(c31) + fabs(c32) + fabs(c33);
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            int red = (c11*qRed(rgb11) + c12*qRed(rgb12) + c13*qRed(rgb13) +
                       c21*qRed(rgb21) + c22*qRed(rgb22) + c23*qRed(rgb23) +
                       c31*qRed(rgb31) + c32*qRed(rgb32) + c33*qRed(rgb33)) / total;
            int green = (c11*qGreen(rgb11) + c12*qGreen(rgb12) + c13*qGreen(rgb13) +
                         c21*qGreen(rgb21) + c22*qGreen(rgb22) + c23*qGreen(rgb23) +
                         c31*qGreen(rgb31) + c32*qGreen(rgb32) + c33*qGreen(rgb33)) / total;
            int blue = (c11*qBlue(rgb11) + c12*qBlue(rgb12) + c13*qBlue(rgb13) +
                        c21*qBlue(rgb21) + c22*qBlue(rgb22) + c23*qBlue(rgb23) +
                        c31*qBlue(rgb31) + c32*qBlue(rgb32) + c33*qBlue(rgb33)) / total;

            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionSobel_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            int red_gx = ((-1)*qRed(rgb11) + 1*qRed(rgb13) +
                       (-2)*qRed(rgb21) +  2*qRed(rgb23) +
                       (-1)*qRed(rgb31) +  1*qRed(rgb33));
            int red_gy = ((-1)*qRed(rgb11) + (-2)*qRed(rgb12) + (-1)*qRed(rgb13) +
                         1*qRed(rgb31) + 2*qRed(rgb32) + 1*qRed(rgb33));
            int red = sqrt(red_gx*red_gx + red_gy*red_gy);

            int green_gx = ((-1)*qGreen(rgb11) + 1*qGreen(rgb13) +
                       (-2)*qGreen(rgb21) +  2*qGreen(rgb23) +
                       (-1)*qGreen(rgb31) +  1*qGreen(rgb33));
            int green_gy = ((-1)*qGreen(rgb11) + (-2)*qGreen(rgb12) + (-1)*qGreen(rgb13) +
                         1*qGreen(rgb31) + 2*qGreen(rgb32) + 1*qGreen(rgb33));
            int green = sqrt(green_gx*green_gx + green_gy*green_gy);

            int blue_gx = ((-1)*qBlue(rgb11) + 1*qBlue(rgb13) +
                       (-2)*qBlue(rgb21) +  2*qBlue(rgb23) +
                       (-1)*qBlue(rgb31) +  1*qBlue(rgb33));
            int blue_gy = ((-1)*qBlue(rgb11) + (-2)*qBlue(rgb12) + (-1)*qBlue(rgb13) +
                         1*qBlue(rgb31) + 2*qBlue(rgb32) + 1*qBlue(rgb33));
            int blue = sqrt(blue_gx*blue_gx + blue_gy*blue_gy);

            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionLaplace_triggered()
{
    int width = image->width();
    int height = image->height();

    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            int red = ((-1)*qRed(rgb11) + (-1)*qRed(rgb12) + (-1)*qRed(rgb13) +
                       (-1)*qRed(rgb21) + 8*qRed(rgb22) + (-1)*qRed(rgb23) +
                       (-1)*qRed(rgb31) + (-1)*qRed(rgb32) + (-1)*qRed(rgb33)) / 16;
            int green = ((-1)*qGreen(rgb11) + (-1)*qGreen(rgb12) + (-1)*qGreen(rgb13) +
                         (-1)*qGreen(rgb21) + 8*qGreen(rgb22) + (-1)*qGreen(rgb23) +
                         (-1)*qGreen(rgb31) + (-1)*qGreen(rgb32) + (-1)*qGreen(rgb33)) / 16;
            int blue = ((-1)*qBlue(rgb11) + (-1)*qBlue(rgb12) + (-1)*qBlue(rgb13) +
                        (-1)*qBlue(rgb21) + 8*qBlue(rgb22) + (-1)*qBlue(rgb23) +
                        (-1)*qBlue(rgb31) + (-1)*qBlue(rgb32) + (-1)*qBlue(rgb33)) / 16;

            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionCanny_triggered()
{
    on_actionGauss_triggered();

    int width = image->width();
    int height = image->height();

    int ** angle = new int*[width];
    for(int i = 0; i < width; i++)
    {
        angle[i] = new int[height];
    }

    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);

    QRgb rgb11, rgb12, rgb13, rgb21, rgb22, rgb23, rgb31, rgb32, rgb33;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            int red_gx = ((-1)*qRed(rgb11) + 1*qRed(rgb13) +
                       (-2)*qRed(rgb21) +  2*qRed(rgb23) +
                       (-1)*qRed(rgb31) +  1*qRed(rgb33));
            int red_gy = ((-1)*qRed(rgb11) + (-2)*qRed(rgb12) + (-1)*qRed(rgb13) +
                         1*qRed(rgb31) + 2*qRed(rgb32) + 1*qRed(rgb33));
            int red = sqrt(red_gx*red_gx + red_gy*red_gy);

            int green_gx = ((-1)*qGreen(rgb11) + 1*qGreen(rgb13) +
                       (-2)*qGreen(rgb21) +  2*qGreen(rgb23) +
                       (-1)*qGreen(rgb31) +  1*qGreen(rgb33));
            int green_gy = ((-1)*qGreen(rgb11) + (-2)*qGreen(rgb12) + (-1)*qGreen(rgb13) +
                         1*qGreen(rgb31) + 2*qGreen(rgb32) + 1*qGreen(rgb33));
            int green = sqrt(green_gx*green_gx + green_gy*green_gy);

            int blue_gx = ((-1)*qBlue(rgb11) + 1*qBlue(rgb13) +
                       (-2)*qBlue(rgb21) +  2*qBlue(rgb23) +
                       (-1)*qBlue(rgb31) +  1*qBlue(rgb33));
            int blue_gy = ((-1)*qBlue(rgb11) + (-2)*qBlue(rgb12) + (-1)*qBlue(rgb13) +
                         1*qBlue(rgb31) + 2*qBlue(rgb32) + 1*qBlue(rgb33));
            int blue = sqrt(blue_gx*blue_gx + blue_gy*blue_gy);

            angle[i][j] = atan(fabs(red_gy + green_gy + blue_gy) / fabs(red_gx + green_gx + blue_gx)) * 180 / PI;

            newImage->setPixel(i, j, qRgb(red, green, blue));
        }
    }

    width = width - 2;
    height = height - 2;

    *image = *newImage;

    for(int i = 1; i < (width - 1); i++)
    {
        for(int j = 1; j < (height - 1); j++)
        {
            rgb11 = image->pixel(i - 1, j - 1);
            rgb12 = image->pixel(i, j - 1);
            rgb13 = image->pixel(i + 1, j - 1);
            rgb21 = image->pixel(i - 1, j);
            rgb22 = image->pixel(i, j);
            rgb23 = image->pixel(i + 1, j);
            rgb31 = image->pixel(i - 1, j + 1);
            rgb32 = image->pixel(i, j + 1);
            rgb33 = image->pixel(i + 1, j + 1);

            int current_angle = angle[i][j];

            if ((current_angle >= 0 && current_angle < 23) || (current_angle >= 158 && current_angle < 180))
            {
                if(qRed(rgb22) < qRed(rgb21) || qRed(rgb22) < qRed(rgb23))
                {
                    rgb22 = qRgb(0, 0, 0);
                }

            } else if (current_angle >= 23 && current_angle < 68) {
                if(qRed(rgb22) < qRed(rgb13) || qRed(rgb22) < qRed(rgb31))
                {
                    rgb22 = qRgb(0, 0, 0);
                }

            } else if (current_angle >= 68 && current_angle < 113 ) {
                if(qRed(rgb22) < qRed(rgb12) || qRed(rgb22) < qRed(rgb32))
                {
                    rgb22 = qRgb(0, 0, 0);
                }

            } else if (current_angle >= 113 && current_angle < 158) {
                if(qRed(rgb22) < qRed(rgb11) || qRed(rgb22) < qRed(rgb33))
                {
                    rgb22 = qRgb(0, 0, 0);
                }
            }

            newImage->setPixel(i, j, rgb22);
        }
    }

    *image = *newImage;

//    width = width - 2;
//    height = height - 2;

//    QRgb * line;
//    int total_pixels = 0;
//    int greylevel[256] = {0};
//    double probabilities[256] = {0};
//    double w0 = 0, w1 = 0, u0 = 0, u1 = 0, last_sigma2 = 0,sigma2 = 0;
//    int t = 0, final_t = 0 , min = 0;

//    for(int y = 0; y<height; y++){
//        line = (QRgb *)image->scanLine(y);

//        for(int x = 0; x<width; x++){
//            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
//            total_pixels++;
//            greylevel[average]++;
//        }

//    }

//    for(int x = 0; x<256; x++)
//    {
//        probabilities[x] = double(greylevel[x]) / total_pixels;
//    }

//    for(t = 1; t<256; t++)
//    {
//        w0 = 0, w1 = 0, u0 = 0, u1 = 0;
//        for(int x = 0; x<t; x++)
//        {
//            w0 = w0 + probabilities[x];
//        }
//        for(int x = t; x<256; x++)
//        {
//            w1 = w1 + probabilities[x];
//        }
//        for(int x = 0; x<t; x++)
//        {
//            u0 = u0 + x*probabilities[x]/w0;
//        }
//        for(int x = t; x<256; x++)
//        {
//            u1 = u1 + x*probabilities[x]/w1;
//        }
//        sigma2 = w0*w1*((u0 - u1)*(u0 - u1));
//        if (sigma2 > last_sigma2)
//        {
//            final_t = t;
//        }
//        last_sigma2 = sigma2;
//    }

//    min = final_t / 2;

//    for(int y = 0; y<height; y++){
//        line = (QRgb *)image->scanLine(y);

//        for(int x = 0; x<width; x++){
//            int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x]))/3;
//            if (average >= min && average <= final_t)
//            {
//                newImage->setPixel(x, y, line[x]);
//            } else {
//                newImage->setPixel(x, y, qRgb(0, 0, 0));
//            }

//        }

//    }

//    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;

    for(int i = 0; i < width; i++)
    {
        delete angle[i];
    }
    delete angle;
}

void Notepad::on_actionDilation_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            newImage->setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            if(qRed(image->pixel(i, j)) == 255)
            {
                for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
                {
                    for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                    {
                        if(structure_element[origin - 1 + x + 3*y] == 1)
                        {
                            newImage->setPixel(i + x, j + y, qRgb(255, 255, 255));
                        }
                    }
                }
            }
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionErosion_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            newImage->setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int flag = 1;

            for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
            {
                for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                {
                    if((structure_element[origin - 1 + x + 3*y] == 1) && (qRed(image->pixel(i + x, j + y)) != 255))
                    {
                        flag = 0;
                    }
                }
            }

            if (flag == 1)
            {
                newImage->setPixel(i , j , qRgb(255, 255, 255));
            }
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionOpening_triggered()
{
    on_actionErosion_triggered();
    on_actionDilation_triggered();
}

void Notepad::on_actionClosing_triggered()
{
    on_actionDilation_triggered();
    on_actionErosion_triggered();
}

void Notepad::on_actionDistance_Transform_triggered()
{
    int width = image->width();
    int height = image->height();

    QImage before_image(width, height, QImage::Format_ARGB32);
    QImage after_image(width, height, QImage::Format_ARGB32);
    before_image = after_image = *image;

    int ** distance = new int*[width];
    for(int i = 0; i < width; i++)
    {
        distance[i] = new int[height];
        for(int j = 0; j < height; j++)
        {
            distance[i][j] = 0;
        }
    }

    int flag = 1;
    int times = 1;

    while(flag == 1)
    {
        flag = 0;

        before_image = after_image;
        after_image = Erosion(before_image);

        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                if(qRed(before_image.pixel(i, j)) == 255 && qRed(after_image.pixel(i, j)) == 0)
                {
                    distance[i][j] = times;
                }
            }
        }

        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                if(qRed(after_image.pixel(i, j)) == 255)
                {
                    flag = 1;
                }
            }
        }

        times++;
        qDebug() << times;
    }

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            int gray_level = distance[i][j];
            image->setPixel(i, j, qRgb(gray_level, gray_level, gray_level));
        }
    }

    imageLabel->setPixmap(QPixmap::fromImage(*image));

    for(int i = 0; i < width; i++)
    {
        delete distance[i];
    }
    delete distance;
}

void Notepad::on_actionHit_or_Miss_triggered()
{
    *image = HitOrMiss(*image);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionThinning_triggered()
{
    *image = Intersection(*image, Complement(HitOrMiss(*image)));
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionThickening_triggered()
{
    *image = Union(*image, HitOrMiss(*image));
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionSkeleton_triggered()
{
    *image = Subtract(Erosion(*image), Opening(Erosion(*image)));
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionReconstruction_triggered()
{
    int width = image->width();
    int height = image->height();

    QImage V(*image);
    QImage M(width, height, QImage::Format_ARGB32);
    QImage T(*image);
    M = Opening(V);

    while(!(T == M))
    {
        T = M;
        M = Intersection(V, Dilation(M));
    }

    *image = T;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
}

void Notepad::on_actionDilation_2_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            newImage->setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int max = 0;

            for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
            {
                for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                {
                    if(structure_element[origin - 1 + x + 3*y] == 1 && qRed(image->pixel(i + x, j + y)) > max)
                    {
                        max = qRed(image->pixel(i + x, j + y));
                    }
                }
            }

            newImage->setPixel(i, j, qRgb(max, max, max));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionErosion_2_triggered()
{
    int width = image->width();
    int height = image->height();
    QImage * newImage = new QImage(width, height, QImage::Format_ARGB32);
    int structure_element[9];
    structure_element[0] = ui->structuringElement11->toPlainText().toInt();
    structure_element[1] = ui->structuringElement12->toPlainText().toInt();
    structure_element[2] = ui->structuringElement13->toPlainText().toInt();
    structure_element[3] = ui->structuringElement21->toPlainText().toInt();
    structure_element[4] = ui->structuringElement22->toPlainText().toInt();
    structure_element[5] = ui->structuringElement23->toPlainText().toInt();
    structure_element[6] = ui->structuringElement31->toPlainText().toInt();
    structure_element[7] = ui->structuringElement32->toPlainText().toInt();
    structure_element[8] = ui->structuringElement33->toPlainText().toInt();

    int origin = ui->buttonGroup->checkedButton()->objectName().toStdString()[11] - '0';

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            newImage->setPixel(i, j, qRgb(0, 0, 0));
        }
    }

    for(int i = 0; i<width; i++)
    {
        for(int j = 0; j<height; j++)
        {
            int min = 255;

            for(int x = (0 - ((origin - 1) % 3)); x < (3 - ((origin - 1) % 3)); x++)
            {
                for(int y = (0 - ((origin - 1) / 3)); y < (3 - ((origin - 1) / 3)); y++)
                {
                    if(structure_element[origin - 1 + x + 3*y] == 1 && qRed(image->pixel(i + x, j + y)) < min)
                    {
                        min = qRed(image->pixel(i + x, j + y));
                    }
                }
            }

            newImage->setPixel(i, j, qRgb(min, min, min));
        }
    }

    *image = *newImage;
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    delete newImage;
}

void Notepad::on_actionOpening_2_triggered()
{
    on_actionErosion_2_triggered();
    on_actionDilation_2_triggered();
}

void Notepad::on_actionClosing_2_triggered()
{
    on_actionDilation_2_triggered();
    on_actionErosion_2_triggered();
}

void Notepad::on_actionReconstruction_2_triggered()
{
    int i = 0;
    while(i < 10)
    {
        *image = GrayDilation(*image);
        i++;
    }

    imageLabel->setPixmap(QPixmap::fromImage(*image));
}


