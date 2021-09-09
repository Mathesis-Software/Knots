include (../qmake.pri)

QT -= core gui

TEMPLATE = app

CONFIG -= qt
config += console

QMAKE_LIBDIR += ../../ke ../../math
LIBS += -lke -lmath

SOURCES = *.cpp
