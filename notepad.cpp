#include "notepad.h"
#include "ui_notepad.h"

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
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(imageLabel);

    imageLabel_2 = new QLabel;
    imageLabel_2->setAlignment(Qt::AlignTop);
    ui->scrollArea_2->setBackgroundRole(QPalette::Dark);
    ui->scrollArea_2->setWidget(imageLabel);

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
            if (average > min && average <= max)
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
            if (average > min && average <= max)
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
            }else {
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
