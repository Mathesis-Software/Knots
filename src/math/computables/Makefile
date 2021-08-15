ROOTDIR = ../..

all: computables.a

include $(ROOTDIR)/config.mk

computables.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
