ROOTDIR = ../..

all: knotSurface.a

include $(ROOTDIR)/config.mk

knotSurface.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
