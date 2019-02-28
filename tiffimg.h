#ifndef TIFFIMG_H
#define TIFFIMG_H

#include <QWidget>
#include <QGestureEvent>
#include <QPinchGesture>
#include <QImage>
#include <QWheelEvent>
#include <QPanGesture>
#include <QMouseEvent>
#include "opencv2/opencv.hpp"//添加Opencv相关头文件

using namespace cv;

class tiffImg : public QWidget
{
    Q_OBJECT

public:
    tiffImg(QWidget *parent);
    tiffImg(QWidget *parent, QString path);
    ~tiffImg();

    void    loadImg(QString path);        //输入路径加载图片
    void    reloadImg(QImage newImage);   //对原来大小的图片做处理，重新显示原图

private:
    ///// original image /////
    QImage image;         //原始图用来计算尺寸
    QImage scaleImage;    //实际显示
    ////mouse position////
    bool mousePress;
//    bool m_bMovble;
    QPoint start;
    ///// image position and scale /////
    float imgX;
    float imgY;
    float scaleFactor;
    int horizontalOffset;
    int verticalOffset;

    int m_iXRange;
    int m_iYRange;

protected:
    void paintEvent(QPaintEvent *);
    /////Gesture////
#ifdef ANDROID
    bool event(QEvent *event);
    void pinchTriggered(QPinchGesture *); //缩放
    bool gestureEvent(QGestureEvent *);
#endif
    /////mouse events/////
#ifdef WIN32
    void wheelEvent(QWheelEvent *);
#endif
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);


    cv::Mat QImageToMat(QImage image);
    QImage MatToQImage(cv::Mat mtx);
};

#endif // TIFFIMG_H
