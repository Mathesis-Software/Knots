ROOTDIR = ../..

all: 3dWindow.a

include $(ROOTDIR)/config.mk

3dWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
