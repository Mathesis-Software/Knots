ROOTDIR = ../..

all: setupColor.a

include $(ROOTDIR)/config.mk

setupColor.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
