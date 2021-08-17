ROOTDIR = ../..

all: KnotEditor

include $(ROOTDIR)/config.mk

OTHERS = ../window/window.a ../widget/widget.a ../setValue/setValue.a $(ROOTDIR)/math/computables/computables.a $(ROOTDIR)/math/knotSurface/knotSurface.a $(ROOTDIR)/math/knotWrapper/knotWrapper.a $(ROOTDIR)/math/knot/knot.a $(ROOTDIR)/math/diagramEditor/diagramEditor.a $(ROOTDIR)/math/diagram/diagram.a $(ROOTDIR)/math/seifert/seifert.a $(ROOTDIR)/gl/surface/surface.a

KnotEditor: $(OBJECTS) $(OTHERS)
	@echo -n 'Linking $@ ...'
	@$(CC) $(DEBUGOPT) -o $@ $^ $(LIBS)
	@echo ' OK'
