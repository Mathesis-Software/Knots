ROOTDIR = ../..

all: gl.a

include $(ROOTDIR)/config.mk

gl.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
