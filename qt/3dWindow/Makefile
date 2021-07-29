ROOTDIR = ../..

all: 3dWindow.a

include $(ROOTDIR)/config.mk

OBJECTS += 3dWindow.moc.o

3dWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
