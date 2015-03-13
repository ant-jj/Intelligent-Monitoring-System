#-------------------------------------------------
#
# Project created by QtCreator 2014-04-30T21:53:27
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpservergui-en
TEMPLATE = app

INCLUDEPATH += D:/sdk/opencv/build/include \
INCLUDEPATH += D:/sdk/opencv/build/include/opencv

# debug

#LIBS += -LD:/sdk/opencv/build/x86/vc12/lib \
#    -lopencv_calib3d249d \
#    -lopencv_contrib249d \
#    -lopencv_core249d \
#    -lopencv_features2d249d \
#    -lopencv_flann249d \
#    -lopencv_gpu249d \
#    -lopencv_highgui249d \
#    -lopencv_imgproc249d \
#    -lopencv_legacy249d \
#    -lopencv_ml249d \
#    -lopencv_nonfree249d \
#    -lopencv_objdetect249d \
#    -lopencv_ocl249d \
#    -lopencv_photo249d \
#    -lopencv_stitching249d \
#    -lopencv_superres249d \
#    -lopencv_ts249d \
#    -lopencv_video249d \
#    -lopencv_videostab249d

# release
LIBS += -LD:/sdk/opencv/build/x86/vc12/lib \
    -lopencv_calib3d249 \
    -lopencv_contrib249 \
    -lopencv_core249 \
    -lopencv_features2d249 \
    -lopencv_flann249 \
    -lopencv_gpu249 \
    -lopencv_highgui249 \
    -lopencv_imgproc249 \
    -lopencv_legacy249 \
    -lopencv_ml249 \
    -lopencv_nonfree249 \
    -lopencv_objdetect249 \
    -lopencv_ocl249 \
    -lopencv_photo249 \
    -lopencv_stitching249 \
    -lopencv_superres249 \
    -lopencv_ts249 \
    -lopencv_video249 \
    -lopencv_videostab249

INCLUDEPATH += $$PWD
INCLUDEPATH += d:/install/boost_1_56_0

LIBS += -Ld:/install/boost_1_56_0/lib32-msvc-12.0  \
	-lboost_system-vc120-mt-1_56 \
	-lboost_thread-vc120-mt-1_56 \
	-llibboost_system-vc120-mt-1_56 \
	-llibboost_thread-vc120-mt-1_56


LIBS +=-L$$PWD/lib  \
       -llibconfig \
       -llibconfig++

SOURCES += main.cpp\
	dialog.cpp \
    package_bgs/FrameDifferenceBGS.cpp \
    package_bgs/MixtureOfGaussianV2BGS.cpp \
    package_tracking/BlobTracking.cpp \
    package_tracking/cvblob/cvaux.cpp \
    package_tracking/cvblob/cvblob.cpp \
    package_tracking/cvblob/cvcolor.cpp \
    package_tracking/cvblob/cvcontour.cpp \
    package_tracking/cvblob/cvlabel.cpp \
    package_tracking/cvblob/cvtrack.cpp \
    package_analysis/VehicleCouting.cpp \
    package_bgs/pt/PBAS.cpp \
    package_bgs/pt/PixelBasedAdaptiveSegmenter.cpp \
    particle/condens.cpp \
    particle/filter.cpp \
    particle/hist.cpp \
    particle/lbp.cpp \
    particle/selector.cpp \
    CompressiveTracking/CompressiveTracker.cpp \
    miltracker/cv_onlineboosting.cpp \
    miltracker/cv_onlinemil.cpp \
    miltracker/object_tracker.cpp \
    miltracker/Window.cpp \
    scene/scene1/formscene1.cpp \
    scene/scene2/formscene2.cpp \
    scene/scene3/formscene3.cpp \
    moveableframelesswindow/moveableframelesswindow.cpp \
    panel/carsettingpaneldialog.cpp \
    panel/datasourcedialog.cpp \
    panel/detectmethoddialog.cpp \
    panel/firstframedetectmethodssetdialog.cpp \
    panel/firstpaneldialog.cpp \
    panel/trakerselectdialog.cpp \
    panel/whattotrackselectdialog.cpp \
    worker/ctworker.cpp \
    worker/detectfirstframeobjecttoinitrectworker.cpp \
    worker/getnetframeworker.cpp \
    worker/milworker.cpp \
    worker/networkworker.cpp \
    worker/worker.cpp \
    lib/parameters.cpp
#    tracker/particle/condens.cpp \
#    tracker/particle/filter.cpp \
#    tracker/particle/hist.cpp \
#    tracker/particle/lbp.cpp \
#    tracker/particle/selector.cpp


HEADERS  += dialog.h \
    package_bgs/MixtureOfGaussianV2BGS.h \
    package_bgs/FrameDifferenceBGS.h \
    package_tracking/BlobTracking.h \
    package_tracking/cvblob/cvblob.h \
    package_analysis/VehicleCouting.h \
    package_bgs/pt/PBAS.h \
    package_bgs/pt/PixelBasedAdaptiveSegmenter.h \
    particle/condens.h \
    particle/filter.h \
    particle/hist.h \
    particle/lbp.h \
    particle/selector.h \
    particle/state.h \
    CompressiveTracking/CompressiveTracker.h \
    miltracker/cv_onlineboosting.h \
    miltracker/cv_onlinemil.h \
    miltracker/object_tracker.h \
    miltracker/Window.h \
    tracktype.h \
    scene/scene1/formscene1.h \
    scene/scene2/formscene2.h \
    scene/scene3/formscene3.h \
    moveableframelesswindow/moveableframelesswindow.h \
    panel/carsettingpaneldialog.h \
    panel/datasourcedialog.h \
    panel/detectmethoddialog.h \
    panel/firstframedetectmethodssetdialog.h \
    panel/firstpaneldialog.h \
    panel/trakerselectdialog.h \
    panel/whattotrackselectdialog.h \
    worker/ctworker.h \
    worker/detectfirstframeobjecttoinitrectworker.h \
    worker/getnetframeworker.h \
    worker/milworker.h \
    worker/networkworker.h \
    worker/worker.h \
    lib/libconfig.h \
    lib/libconfig.h++ \
    lib/parameters.h
#    tracker/particle/condens.h \
#    tracker/particle/filter.h \
#    tracker/particle/hist.h \
#    tracker/particle/lbp.h \
#    tracker/particle/selector.h \
#    tracker/particle/state.h


FORMS    += dialog.ui \
    scene/scene1/formscene1.ui \
    scene/scene2/formscene2.ui \
    scene/scene3/formscene3.ui \
    panel/carsettingpaneldialog.ui \
    panel/datasourcedialog.ui \
    panel/detectmethoddialog.ui \
    panel/firstframedetectmethodssetdialog.ui \
    panel/firstpaneldialog.ui \
    panel/trakerselectdialog.ui \
    panel/whattotrackselectdialog.ui

RESOURCES += \
    myres.qrc
