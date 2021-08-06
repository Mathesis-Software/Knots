ROOTDIR = ../..

all: KnotEditor

include $(ROOTDIR)/config.mk

OTHERS = ../diagram/diagram.a ../knotWindow/knotWindow.a ../3dWindow/3dWindow.a ../abstractWindow/abstractWindow.a ../setValue/setValue.a ../setupColor/setupColor.a $(ROOTDIR)/math/knot/knot.a $(ROOTDIR)/math/diagram/diagram.a $(ROOTDIR)/math/seifert/seifert.a $(ROOTDIR)/gl/surface/surface.a

KnotEditor: $(OBJECTS) $(OTHERS)
	@echo -n 'Linking $@ ...'
	@$(CC) $(DEBUGOPT) -o $@ $^ $(LIBS)
	@echo ' OK'
