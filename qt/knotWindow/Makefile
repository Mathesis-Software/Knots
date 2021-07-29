ROOTDIR = ../..

all: knotWindow.a

include $(ROOTDIR)/config.mk

OBJECTS += knotWindow.moc.o knotWindow_math.moc.o knotWindow_param.moc.o

knotWindow.a: $(OBJECTS)
	@echo -n 'Linking $@ ...'
	@$(RM) $@
	@$(AR) $@ $^
	@echo ' OK'
