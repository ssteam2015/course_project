#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T16:47:07
#
#-------------------------------------------------

QT       += core gui

TARGET = testVideo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    v4l2capture.cpp \
    SsbSipH264Encode.c \
    performance.c \
    LogMsg.c \
    rtpsend.cpp \
    SsbSipH264Decode.c \
    H264Frames.c \
    FrameExtractor.c \
    FileRead.c \
    Play.cpp \
    recordthread.cpp \
    transportthread.cpp

HEADERS  += widget.h \
    v4l2capture.h \
    MFCH264Encoder.h \
    SsbSipH264Encode.h \
    s3c_pp.h \
    post.h \
    performance.h \
    MfcDrvParams.h \
    MfcDriver.h \
    mfc.h \
    LogMsg.h \
    lcd.h \
    rtpsend.h \
    SsbSipH264Decode.h \
    Play.h \
    H264Frames.h \
    FrameExtractor.h \
    FileRead.h \
    recordthread.h \
    transportthread.h

FORMS    += widget.ui
LIBS += -L./lib -ljrtp -ljthread -lpthread
