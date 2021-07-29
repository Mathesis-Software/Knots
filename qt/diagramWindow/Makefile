ROOTDIR = ../..

all: diagramWindow.a

include $(ROOTDIR)/config.mk

diagramWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
