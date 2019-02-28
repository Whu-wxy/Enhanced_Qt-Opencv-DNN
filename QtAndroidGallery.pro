
QT       += core gui
android{
QT       +=androidextras
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAndroidGallery
TEMPLATE = app


SOURCES += main.cpp \
    dialog.cpp \
    tiffimg.cpp \
    detector.cpp \
    androidsetup.cpp

HEADERS  += \
    dialog.h \
    tiffimg.h \
    detector.h \
    androidsetup.h

FORMS    +=

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/src/com/amin/QtAndroidGallery/QtAndroidGallery.java \
    android-sources/background.jpg \
    src/background.jpg

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

RESOURCES += \
    src.qrc




android {
ANDROID_OPENCV = D:/OpenCV-android-sdk/sdk/native

INCLUDEPATH += \
$$ANDROID_OPENCV/jni/include/opencv    \
$$ANDROID_OPENCV/jni/include/opencv2    \
$$ANDROID_OPENCV/jni/include            \
#$$ANDROID_ZBAR/include  \

LIBS += $$ANDROID_OPENCV/libs/armeabi-v7a/libopencv_java3.so \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_ml.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_objdetect.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_calib3d.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_video.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_features2d.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_highgui.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_dnn.a  \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_flann.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_imgproc.a \
$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_core.a     \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libIlmImf.a  \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjpeg-turbo.a \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibpng.a \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibtiff.a \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/liblibjasper.a \
$$ANDROID_OPENCV/3rdparty/libs/armeabi-v7a/libtbb.a \
#-lopencv_java3   \
#$$ANDROID_ZBAR/libs\armeabi-v7a/libiconv.so  \
#$$ANDROID_ZBAR/libs\armeabi-v7a/libzbarjni.so    \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_contrib.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_legacy.a \
#$$ANDROID_OPENCV/staticlibs/armeabi-v7a/libopencv_androidcamera.a \

data.files += ssd_mobilenet_v1_coco_11_06_2017/frozen_inference_graph.pb
data.files += ssd_mobilenet_v1_coco_11_06_2017/ssd_mobilenet_v1_coco.pbtxt
data.path = /assets/ssd_mobilenet_v1_coco_11_06_2017/
INSTALLS += data


}

#contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    ANDROID_EXTRA_LIBS = \
#        E:/HZF/HZFProject/QtTest2/libnative_camera_r4.0.3.so \
#        E:/HZF/HZFProject/QtTest2/libiconv.so \
#        $$PWD/libzbarjni.so
#}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../OpenCV-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java3.so
}

win32 {
INCLUDEPATH += D:\OpenCVMinGW3.4.1\include
LIBS += D:\OpenCVMinGW3.4.1\bin\libopencv_*.dll
}
