include (../qmake.pri)

QT += core gui widgets openglwidgets network

TARGET = KnotEditor
TEMPLATE = app

QMAKE_LIBDIR += ../ke ../math
LIBS += -lke -lmath

DEFINES += 'VERSION=\\"`cat ../VERSION`\\"'

SOURCES = \
	AboutDialog.cpp \
	Application.cpp \
	BaseWindow.cpp \
	DiagramPropertiesDialog.cpp \
	DiagramWidget.cpp \
	DiagramWindow.cpp \
	FileIconProvider.cpp \
	GLWidget.cpp \
	KnotMathDialog.cpp \
	KnotOptionsDialog.cpp \
	KnotWidget.cpp \
	KnotWidget_smooth.cpp \
	KnotWindow.cpp \
	LibraryModel.cpp \
	LibraryWindow.cpp \
	NetworkManager.cpp \
	StartWindow.cpp \
	Window.cpp \
	main.cpp
HEADERS = *.h
RESOURCES = *.qrc

macx {
	QMAKE_INFO_PLIST = Info.plist
	icons.files = ../../images/file_knt.icns ../../images/file_dgr.icns
	icons.path = Contents/Resources
	QMAKE_BUNDLE_DATA += icons
	ICON = ../../images/trefoil.icns
}

unix:!macx {
	target.path = $$INSTALL_PREFIX/bin
	INSTALLS += target
}
