ROOTDIR = ../..

all: setValue.a

include $(ROOTDIR)/config.mk

setValue.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
