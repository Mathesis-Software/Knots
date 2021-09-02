QT += core gui widgets openglwidgets

TARGET = KnotEditor
TEMPLATE = app

QMAKE_LIBDIR += ../ke
LIBS += -lke

CONFIG += c++17

DEFINES += 'VERSION=\\"`cat ../VERSION`\\"'

QMAKE_CXX = ccache $$QMAKE_CXX
QMAKE_CXXFLAGS += -Wno-unused-command-line-argument

SOURCES = *.cpp
HEADERS = *.h
RESOURCES = *.qrc
ICON = ../../images/trefoil.icns

INCLUDEPATH += /usr/local/Cellar/rapidjson/1.1.0/include
