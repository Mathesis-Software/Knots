include (../qmake.pri)

QT += core gui widgets openglwidgets

TARGET = KnotEditor
TEMPLATE = app

QMAKE_LIBDIR += ../ke ../math
LIBS += -lke -lmath

DEFINES += 'VERSION=\\"`cat ../VERSION`\\"'

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
