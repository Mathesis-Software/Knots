ROOTDIR = ..

all: libke.a

include $(ROOTDIR)/config.mk

libke.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
