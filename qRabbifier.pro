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
    ocv/text/ocr_tesseract.cpp \
    fblackmaker.cpp \
    blackmaker/blackmakerthread.cpp

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
    ocv/text/opencv2/text/ocr.hpp \
    fblackmaker.h \
    blackmaker/blackmakerthread.h

FORMS    += mainwindow.ui \
    frabbifier.ui \
    ftextrecognizer.ui \
    fcartoonizer.ui \
    fblackmaker.ui

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

INCLUDEPATH += $$PWD/../libs/opencv-master/build/include
INCLUDEPATH += $$PWD/../libs/opencv-master/build/include/opencv

LIBS += -L../libs/opencv-master/build/x64/vc15/lib

CONFIG(debug, debug|release) {
    LIBS += -llept173d -ltesseract305d -lopencv_world341d
}

CONFIG(release, debug|release) {
    LIBS += -llept173 -ltesseract305 -lopencv_world341
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
