ROOTDIR = ../../..

all: knot_computables.a

include $(ROOTDIR)/config.mk

knot_computables.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
