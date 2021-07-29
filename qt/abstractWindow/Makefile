ROOTDIR = ../..

all: abstractWindow.a

include $(ROOTDIR)/config.mk

abstractWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
