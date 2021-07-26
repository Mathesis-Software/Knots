ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = diagramWindow_stream.o diagramWindow_base.o \
          diagramWindow_near.o diagramWindow_mouse.o \
	  diagramWindow_draw.o \
	  diagramWindow_simplify.o diagramWindow.moc.o

HEADERS = diagramWindow.h $(ROOTDIR)/math/diagram/diagram.h ../abstractWindow/abstractWindow.h

all: diagramWindow.a

diagramWindow.a: $(OBJECTS)
	$(RM) $@
	$(AR) $@ $^

diagramWindow_stream.o: diagramWindow_stream.cpp $(HEADERS)

diagramWindow_base.o: diagramWindow_base.cpp $(HEADERS) ../knotWindow/knotWindow.h

diagramWindow_draw.o: diagramWindow_draw.cpp $(HEADERS)

diagramWindow_mouse.o: diagramWindow_mouse.cpp $(HEADERS)

diagramWindow_near.o: diagramWindow_near.cpp $(HEADERS)

diagramWindow_simplify.o: diagramWindow_simplify.cpp $(HEADERS)

diagramWindow.moc.cpp: $(HEADERS)
