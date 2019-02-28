#include "detector.h"
#include <QFile>
#include <androidsetup.h>

const char* classNames[]= {"background", "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
"fire hydrant", "background", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "background", "backpack",
"umbrella", "background", "background", "handbag", "tie", "suitcase", "frisbee","skis", "snowboard", "sports ball", "kite", "baseball bat","baseball glove", "skateboard", "surfboard", "tennis racket",
"bottle", "background", "wine glass", "cup", "fork", "knife", "spoon","bowl", "banana",  "apple", "sandwich", "orange","broccoli", "carrot", "hot dog",  "pizza", "donut",
"cake", "chair", "couch", "potted plant", "bed", "background", "dining table", "background", "background", "toilet", "background","tv", "laptop", "mouse", "remote", "keyboard",
"cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "background","book", "clock", "vase", "scissors","teddy bear", "hair drier", "toothbrush"};

Detector::Detector(QObject *parent) : QObject(parent)
{
    bLoad = false;

    inWidth = 300;
    inHeight = 300;
    WHRatio = inWidth / (float)inHeight;


#ifdef Q_OS_ANDROID
    moveFiles();
    AndroidSetup setup;
    QString dataDir = setup.getAppDataDir();
    QString dstName = dataDir + "/frozen_inference_graph.pb";
    weights = dstName;
    dstName = dataDir + "/ssd_mobilenet_v1_coco.pbtxt";
    prototxt = dstName;

#else
    weights = "./ssd_mobilenet_v1_coco_11_06_2017/frozen_inference_graph.pb";
    prototxt = "./ssd_mobilenet_v1_coco_11_06_2017/ssd_mobilenet_v1_coco.pbtxt";
#endif

    if(QFile::exists(weights))
    {
        qDebug()<<"weights file exist";
        net = cv::dnn::readNetFromTensorflow(weights.toStdString(), prototxt.toStdString());
        bLoad = true;
    }
    else
    {
        qDebug()<<"weights file not exist";
        bLoad = false;
    }
}

Detector::~Detector()
{

}

//从assets移动到普通路径下
bool Detector::moveFiles()
{
    AndroidSetup setup;
    QString dataDir = setup.getAppDataDir();
    qDebug()<<"data Dir:"<<dataDir;

    bool bMove = true;
    QString dstName = dataDir + "/frozen_inference_graph.pb";
    if(!QFile::copy("assets:/ssd_mobilenet_v1_coco_11_06_2017/frozen_inference_graph.pb", dstName))
        bMove = false;
    dstName = dataDir + "/ssd_mobilenet_v1_coco.pbtxt";
    if(!QFile::copy("assets:/ssd_mobilenet_v1_coco_11_06_2017/ssd_mobilenet_v1_coco.pbtxt",dstName))
        bMove = false;

    qDebug()<<"move status:"<<bMove;
    return bMove;
}

bool Detector::detect(Mat & frame)
{
    if(!bLoad)
        return false;

    QTime time;
    time.start(); //开始计时，以ms为单位

    Size frame_size = frame.size();

    Size cropSize;
    if (frame_size.width / (float)frame_size.height > WHRatio)
    {
        cropSize = Size(static_cast<int>(frame_size.height * WHRatio),
                        frame_size.height);
    }
    else
    {
        cropSize = Size(frame_size.width,
                        static_cast<int>(frame_size.width / WHRatio));
    }

    Rect crop(Point((frame_size.width - cropSize.width) / 2,
                    (frame_size.height - cropSize.height) / 2),
              cropSize);


    cv::Mat blob = cv::dnn::blobFromImage(frame, 1. / 255, Size(300, 300));
    // 1*3*300*300
    cout << "blob size: " << blob.size << endl;

    net.setInput(blob);
    Mat output = net.forward();
    // 1*1*100*7
    // rows*cols*confidence*矩形框两个点的四个值
    cout << "output size: " << output.size << endl;

    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    frame = frame(crop);
    float confidenceThreshold = 0.50;
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > confidenceThreshold)
        {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

            int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

            QString conf = QString::number(confidence);

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                        (int)(xRightTop - xLeftBottom),
                        (int)(yRightTop - yLeftBottom));

            rectangle(frame, object, Scalar(0, 255, 0), 1);
            //cout << "objectClass:" << objectClass << endl;
            QString label = QString(classNames[objectClass]) + ": " + conf;
            //cout << "label"<<label << endl;
            int baseLine = 0;
            Size labelSize = getTextSize(label.toStdString(), FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                                  Size(labelSize.width, labelSize.height + baseLine)),
                      Scalar(0, 255, 0), CV_FILLED);
            putText(frame, label.toStdString(), Point(xLeftBottom, yLeftBottom),
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
        }
    }

    int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
    qDebug()<<"operate time:"<<time_Diff<<" ms";

    return true;
}
