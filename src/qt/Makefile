ROOTDIR = ..

all: KnotEditor

include $(ROOTDIR)/config.mk

OTHERS = $(ROOTDIR)/math/computables/computables.a $(ROOTDIR)/math/knotWrapper/knotWrapper.a $(ROOTDIR)/math/knot/knot.a $(ROOTDIR)/math/diagramEditor/diagramEditor.a $(ROOTDIR)/math/diagram/diagram.a $(ROOTDIR)/math/seifert/seifert.a $(ROOTDIR)/math/surface/surface.a

KnotEditor: $(OBJECTS) $(OTHERS)
	@echo -n 'Linking $@ ...'
	@$(CC) $(DEBUGOPT) -o $@ $^ $(LIBS)
	@echo ' OK'
