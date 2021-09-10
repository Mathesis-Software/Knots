include (../qmake.pri)

QT -= core gui

TEMPLATE = app

CONFIG -= qt
macx {
	CONFIG -= app_bundle
}
CONFIG += console

QMAKE_LIBDIR += ../../ke ../../math
LIBS += -lke -lmath

SOURCES = *.cpp
