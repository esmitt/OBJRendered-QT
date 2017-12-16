#-------------------------------------------------
#
# Project created by QtCreator 2017-03-28T21:44:45
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = ObjRenderer
TEMPLATE = app

win32:INCLUDEPATH += . "C:/Users/JAY/Desktop/ObjRenderer/" #Put the GLEW .h file path here
win32:LIBS     += -lopengl32 -lglew32 -L"C:\\Users\\JAY\\Desktop\\ObjRenderer\\"#Put the GLEW .lib file path here

unix:CONFIG += link_pkgconfig
PKGCONFIG += glew

SOURCES += main.cpp \
    openglwindow.cpp \
    objwindow.cpp \
    mesh.cpp \
    obj.cpp \
    objfactory.cpp

HEADERS  += \
    openglwindow.h \
    objwindow.h \
    mesh.h \
    material.h \
    materialgroup.h \
    obj.h \
    glew_hack.h \
    objfactory.h

FORMS +=
