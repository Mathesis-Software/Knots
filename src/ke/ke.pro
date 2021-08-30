QT -= core gui

TARGET = ke
TEMPLATE = lib

CONFIG -= qt
CONFIG += c++17 static

QMAKE_CXX = ccache $$QMAKE_CXX
QMAKE_CXXFLAGS += -Wno-unused-command-line-argument

SOURCES = \
	Color.cpp \
	Diagram.cpp \
	DiagramEditor.cpp \
	DiagramEditor_save.cpp \
	DiagramEditor_undo.cpp \
	Diagram_crossing.cpp \
	Diagram_serialization.cpp \
	Diagram_simplify.cpp \
	Diagram_vertex.cpp \
	Knot.cpp \
	KnotSurface.cpp \
	KnotWrapper.cpp \
	Knot_serialization.cpp \
	Knot_smooth.cpp \
	Knot_snapshot.cpp \
	SeifertSurface.cpp \
	Surface.cpp \
	acn.cpp \
	aen.cpp \
	computable.cpp \
	experimental.cpp \
	experimental2.cpp \
	moebius_energy.cpp \
	seifert.cpp \
	seifert_list.cpp \
	seifert_ord.cpp \
	singular.cpp \
	vassiliev_invariant.cpp

HEADERS = \
	Color.h \
	Diagram.h \
	DiagramEditor.h \
	Knot.h \
	KnotSurface.h \
	KnotWrapper.h \
	SeifertSurface.h \
	Surface.h \
	Util_rapidjson.h \
	computable.h \
	computables.h \
	experimental.h \
	primitives.h \
	seifert.h

INCLUDEPATH += /usr/local/Cellar/rapidjson/1.1.0/include
