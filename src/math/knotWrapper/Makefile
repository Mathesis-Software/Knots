ROOTDIR = ../..

all: knotWrapper.a

include $(ROOTDIR)/config.mk

knotWrapper.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
