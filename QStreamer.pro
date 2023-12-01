QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS  __STDC_FORMAT_MACROS
DEFINES += SDL_MAIN_HANDLED
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AVFrameQueue.cpp \
    AVPacketQueue.cpp \
    AudioHandler.cpp \
    BaseQueue.cpp \
    BaseThread.cpp \
    DecodecThread.cpp \
    DexmuxThread.cpp \
    DexmuxUtils.cpp \
    EffectCreator.cpp \
    MainLayout.cpp \
    MainWidget.cpp \
    MultimediaHandler.cpp \
    OpenGLWidget.cpp \
    SpecialEffects.cpp \
    Synchronized.cpp \
    TranscodeUtils.cpp \
    VideoHandler.cpp \
    main.cpp

HEADERS += \
    AVFrameQueue.h \
    AVPacketQueue.h \
    AudioHandler.h \
    BaseQueue.h \
    BaseThread.h \
    DecodecThread.h \
    DexmuxThread.h \
    DexmuxUtils.h \
    EffectCreator.h \
    MainLayout.h \
    MainWidget.h \
    MultimediaHandler.h \
    OpenGLWidget.h \
    SpecialEffects.h \
    Synchronized.h \
    TranscodeUtils.h \
    VideoHandler.h

INCLUDEPATH +=$$PWD/ffmpeg-6.0-full_build-shared/include
INCLUDEPATH += $$PWD/SDL2-2.28.3/include
LIBS += $$PWD/ffmpeg-6.0-full_build-shared/lib/avcodec.lib \
        $$PWD/ffmpeg-6.0-full_build-shared/lib/avfilter.lib \
        $$PWD/ffmpeg-6.0-full_build-shared/lib/avformat.lib \
        $$PWD/ffmpeg-6.0-full_build-shared/lib/avutil.lib \
        $$PWD/ffmpeg-6.0-full_build-shared/lib/postproc.lib \
        $$PWD/ffmpeg-6.0-full_build-shared/lib/swresample.lib \
        $$PWD/ffmpeg-6.0-full_build-shared/lib/swscale.lib \
        $$PWD/SDL2-2.28.3/lib/x64/SDL2.lib  \
        $$PWD/SDL2-2.28.3/lib/x64/SDL2main.lib

FORMS += \
    MainWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fragment.fsh \
    vertex.vsh

RESOURCES += \
    rc.qrc
