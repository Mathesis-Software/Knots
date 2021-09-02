QT += core gui widgets openglwidgets

TARGET = KnotEditor
TEMPLATE = app

QMAKE_LIBDIR += ../ke
LIBS += -lke

CONFIG += c++17

DEFINES += 'VERSION=\\"`cat ../VERSION`\\"'

QMAKE_CXX = ccache $$QMAKE_CXX
QMAKE_CXXFLAGS += -Wno-unused-command-line-argument

SOURCES = \
	AboutWindow.cpp \
	BaseWindow.cpp \
	DiagramWidget.cpp \
	DiagramWindow.cpp \
	FileIconProvider.cpp \
	GLWidget.cpp \
	KnotMathDialog.cpp \
	KnotOptionsDialog.cpp \
	KnotWidget.cpp \
	KnotWidget_smooth.cpp \
	KnotWindow.cpp \
	LibraryWindow.cpp \
	StartWindow.cpp \
	Window.cpp \
	main.cpp
HEADERS = *.h
RESOURCES = *.qrc
ICON = ../../images/trefoil.icns

INCLUDEPATH += /usr/local/Cellar/rapidjson/1.1.0/include
