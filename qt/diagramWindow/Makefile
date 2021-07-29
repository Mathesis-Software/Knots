ROOTDIR = ../..

all: diagramWindow.a

include $(ROOTDIR)/config.mk

OBJECTS += diagramWindow.moc.o

diagramWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
