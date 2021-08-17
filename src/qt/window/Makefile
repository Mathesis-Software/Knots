ROOTDIR = ../..

all: window.a

include $(ROOTDIR)/config.mk

window.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
