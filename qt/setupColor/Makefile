ROOTDIR = ../..

all: setupColor.a

include $(ROOTDIR)/config.mk

OBJECTS += setupColor.moc.o

setupColor.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
