QT       += core gui widgets

TARGET = KnotEditor
TEMPLATE = app

QMAKE_LIBDIR += ../ke
LIBS += -lke

CONFIG += c++17

DEFINES += "VERSION=1.0"

QMAKE_CXX = ccache $$QMAKE_CXX
QMAKE_CXXFLAGS += -Wno-unused-command-line-argument

SOURCES = \
	AboutWindow.cpp \
	DiagramWidget.cpp \
	DiagramWindow.cpp \
	FileIconProvider.cpp \
	GLWidget.cpp \
	KnotMathDialog.cpp \
	KnotOptionsDialog.cpp \
	KnotWidget.cpp \
	KnotWidget_smooth.cpp \
	KnotWindow.cpp \
	ManagerWindow.cpp \
	Window.cpp \
	Window_static.cpp \
	main.cpp

HEADERS = \
	AboutWindow.h \
	DiagramWidget.h \
	DiagramWindow.h \
	FileIconProvider.h \
	GLWidget.h \
	KnotWidget.h \
	KnotWindow.h \
	ManagerWindow.h \
	Window.h

RESOURCES = \
	images.qrc

INCLUDEPATH += /usr/local/Cellar/rapidjson/1.1.0/include
