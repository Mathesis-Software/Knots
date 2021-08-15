ROOTDIR = ../..

all: KnotEditor

include $(ROOTDIR)/config.mk

OTHERS = ../diagram/diagram.a ../knot/knot.a ../gl/gl.a ../abstractWindow/abstractWindow.a ../setValue/setValue.a $(ROOTDIR)/math/computables/computables.a $(ROOTDIR)/math/knotSurface/knotSurface.a $(ROOTDIR)/math/knot/knot.a $(ROOTDIR)/math/diagramEditor/diagramEditor.a $(ROOTDIR)/math/diagram/diagram.a $(ROOTDIR)/math/seifert/seifert.a $(ROOTDIR)/gl/surface/surface.a

KnotEditor: $(OBJECTS) $(OTHERS)
	@echo -n 'Linking $@ ...'
	@$(CC) $(DEBUGOPT) -o $@ $^ $(LIBS)
	@echo ' OK'
