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
