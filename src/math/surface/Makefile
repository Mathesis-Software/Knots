ROOTDIR = ../..

all: surface.a

include $(ROOTDIR)/config.mk

surface.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
