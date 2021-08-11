ROOTDIR = ../..

all: seifert.a

include $(ROOTDIR)/config.mk

seifert.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
