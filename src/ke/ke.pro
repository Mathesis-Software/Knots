QT -= core gui

TARGET = ke
TEMPLATE = lib

CONFIG -= qt
CONFIG += c++17 staticlib

QMAKE_CXX = ccache $$QMAKE_CXX
QMAKE_CXXFLAGS += -Wno-unused-command-line-argument

SOURCES = *.cpp

INCLUDEPATH += /usr/local/Cellar/rapidjson/1.1.0/include
