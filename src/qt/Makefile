ROOTDIR = ..

all: KnotEditor

include $(ROOTDIR)/config.mk

OTHERS = $(ROOTDIR)/ke/libke.a

KnotEditor: $(OBJECTS) $(OTHERS)
	@echo -n 'Linking $@ ...'
	@$(CC) $(DEBUGOPT) -o $@ $^ $(LIBS)
	@echo ' OK'

clean: .clean
	@$(RM) KnotEditor
