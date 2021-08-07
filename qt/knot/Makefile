ROOTDIR = ../..

all: knot.a

include $(ROOTDIR)/config.mk

knot.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
