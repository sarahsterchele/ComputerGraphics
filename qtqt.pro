#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T01:38:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtqt
TEMPLATE = app


SOURCES += application/main.cpp\
simpreader.cpp\
application/client.cpp \
application/window361.cpp \
application/renderarea361.cpp\
renderer.cpp\
xformmatrix.cpp\
z_buffer.cpp\
shader.cpp\
primitives/color.cpp\
primitives/polygonedge.cpp\
primitives/scanline.cpp\

HEADERS  += \
application/drawable.h \
simpreader.h\
application/client.h \
application/window361.h \
application/renderarea361.h\
renderer.h\
xformmatrix.h\
z_buffer.h\
shader.h\
primitives/color.h\
primitives/normal.h\
primitives/light.h\
primitives/vertex.h\
primitives/surface.h\
primitives/ambient.h\
primitives/polygonedge.h\
primitives/scanline.h\