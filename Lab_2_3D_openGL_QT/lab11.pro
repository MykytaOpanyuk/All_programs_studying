#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T21:17:27
#
#-------------------------------------------------

QT       += core gui opengl sql

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab11
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    cube.cpp \
    grid.cpp \
    camera.cpp \
    pyramid.cpp

HEADERS  += mainwindow.h \
    scene.h \
    cube.h \
    sceneobject.h \
    grid.h \
    camera.h \
    figure.h \
    pyramid.h

FORMS    += mainwindow.ui
