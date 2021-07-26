ROOTDIR = ../..

include $(ROOTDIR)/config.mk

OBJECTS = main.o manager.o manager.moc.o about.o about.moc.o \
          icon_provider.moc.o icon_provider.o

OTHERS = ../diagramWindow/diagramWindow.a ../knotWindow/knotWindow.a ../3dWindow/3dWindow.a ../abstractWindow/abstractWindow.a ../setValue/setValue.a ../setupColor/setupColor.a $(ROOTDIR)/math/knot/knot.a $(ROOTDIR)/math/diagram/diagram.a $(ROOTDIR)/math/seifert/seifert.a $(ROOTDIR)/gl/surface/surface.a

all: KnotEditor

KnotEditor: $(OBJECTS) $(OTHERS)
	$(CC) $(DEBUGOPT) -o $@ $^ $(LIBS)

manager.o: manager.cpp manager.h about.h

main.o: main.cpp manager.h icon_provider.h

icon_provider.o: icon_provider.cpp icon_provider.h

about.o: about.cpp about.h $(ROOTDIR)/gl/surface/surface.h $(ROOTDIR)/VERSION
