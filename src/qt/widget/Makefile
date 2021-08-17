ROOTDIR = ../..

all: widget.a

include $(ROOTDIR)/config.mk

widget.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
