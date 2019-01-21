#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T09:29:09
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPicSend
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    iconhelper.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    iconhelper.h \
    mythread.h

FORMS    += mainwindow.ui

RESOURCES += \
    qss.qrc

DISTFILES +=

INCLUDEPATH     +=  D:\FFMPEG\dev\include
LIBS            +=  -LD:\FFMPEG\dev\lib -lavcodec     \
                    -LD:\FFMPEG\dev\lib -lavdevice     \
                    -LD:\FFMPEG\dev\lib -lavfilter     \
                    -LD:\FFMPEG\dev\lib -lavformat     \
                    -LD:\FFMPEG\dev\lib -lavutil       \
                    -LD:\FFMPEG\dev\lib -lpostproc     \
                    -LD:\FFMPEG\dev\lib -lswscale
