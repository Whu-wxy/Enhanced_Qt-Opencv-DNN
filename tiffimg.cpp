#include "tiffimg.h"
#include <QPainter>
#include <QImage>
#include <QDebug>

tiffImg::tiffImg(QWidget *parent)
    : QWidget(parent)
{
    scaleFactor=1;
    horizontalOffset=0;
    verticalOffset=0;

#ifdef Q_OS_ANDROID
    grabGesture(Qt::PinchGesture);
#else
    grabMouse();
#endif
    mousePress=false;
}

tiffImg::tiffImg(QWidget *parent, QString path)
: QWidget(parent)
{
    loadImg(path);
    scaleFactor=1;
    horizontalOffset=0;
    verticalOffset=0;

#ifdef Q_OS_ANDROID
    grabGesture(Qt::PinchGesture);
#else
    grabMouse();
#endif
    mousePress=false;
}

tiffImg::~tiffImg()
{

}

void tiffImg::loadImg(QString path)
{
    if(!image.load(path))
        return;
    scaleImage = image;
    int width0 = this->width();
    int height0 = this->height();
    m_iXRange = width0 * 0.9;
    m_iYRange = height0 * 0.9;

    int imgWidth = image.width();
    int imgHeight = image.height();

    if(width0 < imgWidth || height0 < imgHeight)
    {
        double scaleFactor1 = (double)height0 / width0;
        double scaleFactor2 = (double)imgHeight / imgWidth;
        if(scaleFactor2 > scaleFactor1)
            scaleImage = image.scaledToHeight(height0,Qt::SmoothTransformation);
        else
            scaleImage = image.scaledToWidth(width0,Qt::SmoothTransformation);
    }
    imgX = this->width()/2-scaleImage.width()/2;
    imgY = this->height()/2-scaleImage.height()/2;

    update();
}

void tiffImg::reloadImg(QImage newImage)
{
    image = QImage(newImage);
    scaleImage = QImage(newImage);
    int width0 = this->width();
    int height0 = this->height();
    m_iXRange = width0 * 0.9;
    m_iYRange = height0 * 0.9;

    int imgWidth = image.width();
    int imgHeight = image.height();

    if(width0 < imgWidth || height0 < imgHeight)
    {
        double scaleFactor1 = (double)height0 / width0;
        double scaleFactor2 = (double)imgHeight / imgWidth;
        if(scaleFactor2 > scaleFactor1)
            scaleImage = image.scaledToHeight(height0,Qt::SmoothTransformation);
        else
            scaleImage = image.scaledToWidth(width0,Qt::SmoothTransformation);
    }
    imgX = this->width()/2-scaleImage.width()/2;
    imgY = this->height()/2-scaleImage.height()/2;

    update();
}

void tiffImg::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if(scaleImage.isNull() || image.isNull())
        return;
    p.drawImage(imgX,imgY,scaleImage);
}

#ifdef ANDROID
bool tiffImg::event(QEvent *event)
{
    if(event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

bool tiffImg::gestureEvent(QGestureEvent *e)
{
    if(QGesture *pinch=e->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

//缩放
void tiffImg::pinchTriggered(QPinchGesture *gesture)
{
    if(scaleImage.isNull() || image.isNull())
        return;


     QPinchGesture::ChangeFlags changeFlages=gesture->changeFlags();
     if(changeFlages&QPinchGesture::ScaleFactorChanged)
     {
         qreal value=gesture->scaleFactor();
         if(scaleFactor<=5)
         {
            scaleFactor*=value;
         }
     }
     int width = scaleImage.width();
     int height = scaleImage.height();
     scaleImage = image.scaled(scaleFactor*image.width(),scaleFactor*image.height());

     int deltaX = scaleImage.width()/2 - width/2;
     int deltaY = scaleImage.height()/2 - height/2;
     imgX = imgX - deltaX;
     imgY = imgY - deltaY;
     update();

//     //图完全在窗口内
//     int botRightX = imgX + scaleImage.width();
//     int botRightY = imgY + scaleImage.height();
//     if(imgX >= 0 && imgY >= 0 && botRightX <= this->width() && botRightY <= this->height())
//         m_bMovble = false;
//     if(imgX >= 0 && imgY >= 0 && botRightX <= this->width() && botRightY <= this->height())
//         m_bMovble = true;
}

#elif WIN32

void tiffImg::wheelEvent(QWheelEvent *e)
{
    if(scaleImage.isNull() || image.isNull())
        return;

    if(e->delta()>0)
    {
        if(scaleFactor<=3)
        {
            scaleFactor *= 1.2;
        }
        else
            return;
    }
    else if(e->delta()<0)
    {
        scaleFactor /= 1.2;
    }

    int width = scaleImage.width();
    int height = scaleImage.height();

    QPoint cursorP = cursor().pos();
    QPoint cursor2Widget = QPoint(cursorP.x() - this->pos().x(),cursorP.y() - this->pos().y());
    if(QRect(imgX,imgY,width,height).contains(cursor2Widget))
    {
        int x = cursor2Widget.x() - imgX;
        int y = cursor2Widget.y() - imgY;
//        qDebug()<<imgX<<","<<imgY;
        int deltaX,deltaY;
        if(e->delta()>0)
        {
//             deltaX = x * (scaleFactor - scaleFactor/1.2);
//             deltaY = y * (scaleFactor - scaleFactor/1.2);
            deltaX = x * 1.2 - x;
            deltaY = y * 1.2 - y;
        }
        else
        {
//            deltaX = x * (scaleFactor - scaleFactor*1.2);
//            deltaY = y * (scaleFactor - scaleFactor*1.2);
            deltaX = x / 1.2 - x;
            deltaY = y / 1.2 - y;

        }
//        qDebug()<<deltaX<<","<<deltaY;
//        qDebug()<<"______________";
        scaleImage = image.scaled(scaleFactor*image.width(),scaleFactor*image.height());
//        if(e->delta()>0)
//        {
        imgX = imgX - deltaX;
        imgY = imgY - deltaY;
//        }
//        else
//        {
//            imgX = imgX + deltaX;
//            imgY = imgY + deltaY;
//        }
    }
    else
    {
        scaleImage = image.scaled(scaleFactor*image.width(),scaleFactor*image.height());
        int deltaX = scaleImage.width()/2 - width/2;
        int deltaY = scaleImage.height()/2 - height/2;
        imgX = imgX - deltaX;
        imgY = imgY - deltaY;
    }
    update();
}
#endif

void tiffImg::mousePressEvent(QMouseEvent *e)
{
    if(scaleImage.isNull() || image.isNull())
        return;

    if(e->button()==Qt::LeftButton)
    {
        mousePress=true;
        start.setX(e->x());
        start.setY(e->y());
    }
}

void tiffImg::mouseMoveEvent(QMouseEvent *e)
{
    if(scaleImage.isNull() || image.isNull())
        return;

    if(mousePress==true)
    {
//        if(!m_bMovble)
//            return;

        horizontalOffset = e->x()-start.x();
        verticalOffset = e->y()-start.y();

        int x = imgX;
        int y = imgY;
        x += horizontalOffset;
        y += verticalOffset;

        if(x + scaleImage.width()*0.9 < 0 || x > m_iXRange)
        {
            if(y + scaleImage.height() > 0 && y < m_iYRange)
            {
                imgY += verticalOffset;
                update();
                start.setY(start.y() + verticalOffset);
            }
            return;
        }
        if(y + scaleImage.height()*0.9 < 0 || y > m_iYRange)
        {
            if(x + scaleImage.width() > 0 && x < m_iXRange)
            {
                imgX += horizontalOffset;
                update();
                start.setY(start.x() + horizontalOffset);
            }
            return;
        }

        imgX += horizontalOffset;
        imgY += verticalOffset;
        update();
        start.setX(start.x()+horizontalOffset);
        start.setY(start.y()+verticalOffset);
    }
}

void tiffImg::mouseReleaseEvent(QMouseEvent *e)
{
    if(scaleImage.isNull() || image.isNull())
        return;

    if(e->button()==Qt::LeftButton)
    {
        mousePress=false;
    }

}

cv::Mat tiffImg::QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Grayscale8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage tiffImg::MatToQImage(cv::Mat mtx)
{
    switch (mtx.type())
    {
    case CV_8UC1:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols, QImage::Format_Grayscale8);
        return img;
    }
        break;
    case CV_8UC3:
    {
        qDebug()<<"33333";

        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
        break;
    case CV_8UC4:
    {
        QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 4, QImage::Format_ARGB32);
        return img;
    }
        break;
    default:
    {
        QImage img;
        return img;
    }
        break;
    }
}
