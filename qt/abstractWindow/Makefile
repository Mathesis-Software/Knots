ROOTDIR = ../..

all: abstractWindow.a

include $(ROOTDIR)/config.mk

OBJECTS += abstractWindow.moc.o

abstractWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
