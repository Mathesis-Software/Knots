ROOTDIR = ../..

all: diagram.a

include $(ROOTDIR)/config.mk

diagram.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
