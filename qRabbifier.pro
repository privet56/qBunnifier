QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qRabbifier
TEMPLATE = app

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    DEFINES += DEBUG
}

#would be cool if my libs would also be compiled with this flag
#DEFINES += _ATL_XP_TARGETING

DEFINES += _REENTRANT
DEFINES += NOMINMAX
DEFINES += WIN32_LEAN_AND_MEAN
DEFINES += WIN64_LEAN_AND_MEAN

SOURCES += main.cpp \
    mainwindow.cpp \
    cfg.cpp \
    emitter.cpp \
    logger.cpp \
    logwnd.cpp \
    util/str.cpp \
    util/f.cpp \
    util/tray.cpp \
    frabbifier.cpp \
    ocv/earDetector.cpp \
    ocv/overlayFacemask.cpp \
    ocv/overlayMoustache.cpp \
    ocv/overlayNose.cpp \
    ocv/overlaySunglasses.cpp \
    ocv/ocv.cpp \
    util/w.cpp \
    ftextrecognizer.cpp \
    fcartoonizer.cpp \
    frabbifier/frabbifiermuzzlemodel.cpp \
    frabbifier/frabbifiermuzzleitemdelegate.cpp \
    ocv/segmentocr.cpp \
    ocv/ocr.cpp \
    ocv/text/erfilter.cpp \
    ocv/text/ocr_beamsearch_decoder.cpp \
    ocv/text/ocr_hmm_decoder.cpp \
    ocv/text/ocr_tesseract.cpp

HEADERS  += mainwindow.h \
    res.rc \
    resource.h \
    cfg.h \
    emitter.h \
    logger.h \
    logwnd.h \
    util/str.h \
    globalinclude.h \
    util/f.h \
    util/tray.h \
    frabbifier.h \
    ocv/overlaynose.h \
    ocv/ocv.h \
    util/w.h \
    ftextrecognizer.h \
    fcartoonizer.h \
    frabbifier/frabbifiermuzzlemodel.h \
    frabbifier/frabbifiermuzzleitemdelegate.h \
    ocv/segmentocr.h \
    ocv/ocr.h \
    ocv/text/precomp.hpp \
    ocv/text/opencv2/text.hpp \
    ocv/text/opencv2/text_config.hpp \
    ocv/text/opencv2/text/erfilter.hpp \
    ocv/text/opencv2/text/ocr.hpp

FORMS    += mainwindow.ui \
    frabbifier.ui \
    ftextrecognizer.ui \
    fcartoonizer.ui

RESOURCES += res.qrc

DISTFILES += ico.ico

RC_FILE = res.rc

INCLUDEPATH += ./util
INCLUDEPATH += ./frabbifier
INCLUDEPATH += ./ocv
INCLUDEPATH += ./ocv/text
INCLUDEPATH += ./ocv/text/opencv2
INCLUDEPATH += ./ocv/text/opencv2/text

######### opencv #########

INCLUDEPATH += ../libs/opencv-master/binWin64VS2013
INCLUDEPATH += ../libs/opencv-master/include
INCLUDEPATH += ../libs/opencv-master/include/opencv
INCLUDEPATH += ../libs/opencv-master/modules/core/include
INCLUDEPATH += ../libs/opencv-master/modules/flann/include
INCLUDEPATH += ../libs/opencv-master/modules/imgproc/include
INCLUDEPATH += ../libs/opencv-master/modules/ml/include
INCLUDEPATH += ../libs/opencv-master/modules/photo/include
INCLUDEPATH += ../libs/opencv-master/modules/video/include
INCLUDEPATH += ../libs/opencv-master/modules/imgcodecs/include
INCLUDEPATH += ../libs/opencv-master/modules/shape/include
INCLUDEPATH += ../libs/opencv-master/modules/videoio/include
INCLUDEPATH += ../libs/opencv-master/modules/highgui/include
INCLUDEPATH += ../libs/opencv-master/modules/objdetect/include
INCLUDEPATH += ../libs/opencv-master/modules/superres/include
INCLUDEPATH += ../libs/opencv-master/modules/ts/include
INCLUDEPATH += ../libs/opencv-master/modules/features2d/include
INCLUDEPATH += ../libs/opencv-master/modules/calib3d/include
INCLUDEPATH += ../libs/opencv-master/modules/stitching/include
INCLUDEPATH += ../libs/opencv-master/modules/videostab/include

win32:CONFIG(release, debug|release): LIBS += -L../libs/opencv-master/binWin64VS2013/lib/Release/
win32:CONFIG(debug, debug|release): LIBS += -L../libs/opencv-master/binWin64VS2013/lib/Debug/
win32:CONFIG(release, debug|release): LIBS += -L../libs/opencv-master/binWin64VS2013/3rdparty/lib/Release/
win32:CONFIG(debug, debug|release): LIBS += -L../libs/opencv-master/binWin64VS2013/3rdparty/lib/Debug/

CONFIG(debug, debug|release) {
    LIBS += -llept173d -ltesseract305d -llibtiffd -llibpngd -llibjpegd -lopencv_videostab310d -lopencv_superres310d -lopencv_stitching310d -lopencv_shape310d -lopencv_photo310d -lopencv_objdetect310d -lopencv_calib3d310d -lopencv_features2d310d -lopencv_ml310d -lopencv_highgui310d -lopencv_videoio310d -lopencv_imgcodecs310d -lopencv_flann310d -lopencv_video310d -lopencv_imgproc310d -lopencv_core310d
}

CONFIG(release, debug|release) {
    LIBS += -llept173 -ltesseract305 -llibtiff -llibpng -llibjpeg -lopencv_videostab310 -lopencv_superres310 -lopencv_stitching310 -lopencv_shape310 -lopencv_photo310 -lopencv_objdetect310 -lopencv_calib3d310 -lopencv_features2d310 -lopencv_ml310 -lopencv_highgui310 -lopencv_videoio310 -lopencv_imgcodecs310 -lopencv_flann310 -lopencv_video310 -lopencv_imgproc310 -lopencv_core310
}

# LEPTONICA
DEFINES += LIBLEPT_EXPORTS
DEFINES += HAVE_CONFIG_H
DEFINES += leptonica_EXPORTS

INCLUDEPATH += ../libs/leptonica-1.73
win32:CONFIG(release, debug|release): LIBS += -L../libs/leptonica-1.73/binWin64VS2013/src/Release/
win32:CONFIG(debug, debug|release): LIBS += -L../libs/leptonica-1.73/binWin64VS2013/src/Debug/

# TESSERACT
DEFINES += WIN32
DEFINES += _WINDOWS
DEFINES += TESS_EXPORTS
DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS=1
DEFINES += USE_STD_NAMESPACE=1
DEFINES += tesseract_EXPORTS

INCLUDEPATH += ../libs/tesseract-master/api
INCLUDEPATH += ../libs/tesseract-master/ccmain
INCLUDEPATH += ../libs/tesseract-master/ccutil
INCLUDEPATH += ../libs/tesseract-master/ccstruct
win32:CONFIG(release, debug|release): LIBS += -L../libs/tesseract-master/binWin64VS2013/Release/
win32:CONFIG(debug, debug|release): LIBS += -L../libs/tesseract-master/binWin64VS2013/Debug/
