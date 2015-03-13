TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    condens.cpp \
    filter.cpp \
    hist.cpp \
    lbp.cpp \
    selector.cpp

#INCLUDEPATH += /usr/local/Cellar/opencv/2.4.9/include
#LIBS += -L/usr/local/Cellar/opencv/2.4.9/lib
#LIBS += -lopencv_calib3d \
#-lopencv_contrib \
#-lopencv_core \
#-lopencv_features2d \
#-lopencv_flann \
#-lopencv_gpu \
#-lopencv_highgui \
#-lopencv_imgproc \
#-lopencv_legacy \
#-lopencv_ml \
#-lopencv_objdetect \
#-lopencv_video


INCLUDEPATH += D:/sdk/opencv/build/include \
INCLUDEPATH += D:/sdk/opencv/build/include/opencv

LIBS += -LD:/sdk/opencv/build/x86/vc12/lib \
    -lopencv_calib3d249d \
    -lopencv_contrib249d \
    -lopencv_core249d \
    -lopencv_features2d249d \
    -lopencv_flann249d \
    -lopencv_gpu249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d \
    -lopencv_legacy249d \
    -lopencv_ml249d \
    -lopencv_nonfree249d \
    -lopencv_objdetect249d \
    -lopencv_ocl249d \
    -lopencv_photo249d \
    -lopencv_stitching249d \
    -lopencv_superres249d \
    -lopencv_ts249d \
    -lopencv_video249d \
    -lopencv_videostab249d


include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    condens.h \
    filter.h \
    hist.h \
    lbp.h \
    selector.h \
    state.h

