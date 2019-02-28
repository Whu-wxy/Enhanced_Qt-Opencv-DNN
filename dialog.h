#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QtWidgets>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QButtonGroup>
#include <QStackedWidget>
#include "tiffimg.h"
#include "detector.h"
#include "opencv2/opencv.hpp"//添加Opencv相关头文件
#include <QMessageBox>

using namespace cv;

class Dialog : public tiffImg
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QButtonGroup* btnGroup;
    QPushButton* captureBtn;
    QPushButton* albumBtn;
    QPushButton* processBtn;

    Detector detector;

protected slots:
    void btnClicked(int);
};

#endif // MAINWINDOW_H
