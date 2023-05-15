
TEMPLATE = lib
DEFINES += IVRLOWLEVELSDK_LIBRARY
DEFINES += _CRT_SECURE_NO_WARNINGS
CONFIG  += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#defines the output location
DESTDIR = $$PWD/../libs/IVRLowLevelSDK

SOURCES += \
    IVR_BackStageUI.cpp \
    IVR_BinaryFilter.cpp \
    IVR_CameraTake.cpp \
    IVR_CartoonFilter.cpp \
    IVR_Classifier.cpp \
    IVR_ClassifierClass.cpp \
    IVR_ClassifierFeature.cpp \
    IVR_ClassifierProblem.cpp \
    IVR_ColdFilter.cpp \
    IVR_Config.cpp \
    IVR_DetailFilter.cpp \
    IVR_DilateFilter.cpp \
    IVR_DuoToneFilter.cpp \
    IVR_EmbossFilter.cpp \
    IVR_Frame.cpp \
    IVR_GhostFilter.cpp \
    IVR_GothamFilter.cpp \
    IVR_IFFReader.cpp \
    IVR_IIFReader.cpp \
    IVR_ImageFilter.cpp \
    IVR_ImageHandler.cpp \
    IVR_InvertFilter.cpp \
    IVR_LauncherUI.cpp \
    IVR_LowLevelSDK.cpp \
    IVR_MediaHandler.cpp \
    IVR_OldFilmFilter.cpp \
    IVR_PencilSketchFilter.cpp \
    IVR_RealCamera.cpp \
    IVR_SepiaFilter.cpp \
    IVR_SharpenFilter.cpp \
    IVR_Shrinker.cpp \
    IVR_StylizeFilter.cpp \
    IVR_VideoReader.cpp \
    IVR_VirtualCamera.cpp \
    IVR_WarmFilter.cpp

HEADERS += \
    IVRLowLevelSDK_global.h \
    IVR_BackStageUI.h \
    IVR_BinaryFilter.h \
    IVR_CameraTake.h \
    IVR_CartoonFilter.h \
    IVR_Classifier.h \
    IVR_ClassifierClass.h \
    IVR_ClassifierFeature.h \
    IVR_ClassifierProblem.h \
    IVR_ColdFilter.h \
    IVR_Config.h \
    IVR_DetailFilter.h \
    IVR_DilateFilter.h \
    IVR_DuoToneFilter.h \
    IVR_EmbossFilter.h \
    IVR_Frame.h \
    IVR_GhostFilter.h \
    IVR_GothamFilter.h \
    IVR_IFFReader.h \
    IVR_IIFReader.h \
    IVR_ImageFilter.h \
    IVR_ImageHandler.h \
    IVR_InvertFilter.h \
    IVR_LauncherUI.h \
    IVR_LockFreeQueue.h \
    IVR_LowLevelSDK.h \
    IVR_MediaHandler.h \
    IVR_OldFilmFilter.h \
    IVR_PencilSketchFilter.h \
    IVR_RealCamera.h \
    IVR_SepiaFilter.h \
    IVR_SharpenFilter.h \
    IVR_Shrinker.h \
    IVR_StylizeFilter.h \
    IVR_VideoReader.h \
    IVR_VirtualCamera.h \
    IVR_WarmFilter.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

#OpenCV 4.5.3 - Compiled with MSVC2019 64Bits(With "d" for debug version)
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/ocv454/x64/vc16/lib/ -lopencv_world454
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/ocv454/x64/vc16/lib/ -lopencv_world454d

INCLUDEPATH += $$PWD/../libs/ocv454/include
DEPENDPATH += $$PWD/../libs/ocv454/include

INCLUDEPATH += $$PWD/../libs/Qt/vc16
DEPENDPATH += $$PWD/../libs/Qt/vc16
#QtCore
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Cored
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Core

#QtGUI
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Gui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Guid
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Gui

#Qt Widgets
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Widgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Widgetsd
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Widgets

#Qt Concurrent
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Concurrent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Concurrentd
else:unix: LIBS += -L$$PWD/../libs/Qt/vc16/ -lQt5Concurrent

#FFMPEG API
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavcodec
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavutil
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavdevice
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavfilter
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lavformat
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lswresample
win32: LIBS += -L$$PWD/../libs/ffmpeg/lib/ -lswscale

INCLUDEPATH += $$PWD/../libs/ffmpeg/include
DEPENDPATH += $$PWD/../libs/ffmpeg/include

#quickLZ API
unix|win32: LIBS += -L$$PWD/../libs/quicklz/lib/ -lquicklz150_64_1

INCLUDEPATH += $$PWD/../libs/quicklz/include
DEPENDPATH += $$PWD/../libs/quicklz/include

QT += widgets









