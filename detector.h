#ifndef DETECTOR_H
#define DETECTOR_H

#include <QObject>
#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include<opencv2/dnn.hpp>
#include<QTime>
#include <QDebug>
#include <QDir>

using namespace std;
using namespace cv;

class Detector : public QObject
{
    Q_OBJECT
public:
    explicit Detector(QObject *parent = 0);
    ~Detector();

signals:

public slots:

private:
    size_t inWidth;
    size_t inHeight;
    float WHRatio;

    QString weights;
    QString prototxt;
    dnn::Net net;

    bool bLoad;

public:
    bool    detect(Mat & frame);
    bool    hasLoadNet(){return bLoad; }
    bool    moveFiles();

};

#endif // DETECTOR_H
