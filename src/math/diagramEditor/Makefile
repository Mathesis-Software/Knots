ROOTDIR = ../..

all: diagramEditor.a

include $(ROOTDIR)/config.mk

diagramEditor.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
