ROOTDIR = ../..

all: knotWindow.a

include $(ROOTDIR)/config.mk

knotWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
